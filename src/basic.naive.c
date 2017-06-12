#include "MARG.h"
#include "globals.h"
#include "boards.h"

#define cross_prod(r, a, b) {\
	(r)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1];\
	(r)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2];\
	(r)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0];\
}\

#define dot3(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define dot4(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2] + (a)[3] * (b)[3])

#define scl3(r, v, s) {\
	(r)[0] = (v)[0] * (s);\
	(r)[1] = (v)[1] * (s);\
	(r)[2] = (v)[2] * (s);\
}\

#define scl4(r, v, s) {\
	(r)[0] = (v)[0] * (s);\
	(r)[1] = (v)[1] * (s);\
	(r)[2] = (v)[2] * (s);\
	(r)[3] = (v)[3] * (s);\
}\

#define add3(r, u, v) {\
	(r)[0] = (u)[0] + (v)[0];\
	(r)[1] = (u)[1] + (v)[1];\
	(r)[2] = (u)[2] + (v)[2];\
}\

#define add4(r, u, v) {\
	(r)[0] = (u)[0] + (v)[0];\
	(r)[1] = (u)[1] + (v)[1];\
	(r)[2] = (u)[2] + (v)[2];\
	(r)[3] = (u)[3] + (v)[3];\
}\

#define lerp4(r, v, w, t) {\
	float vs = 1 - t;\
	(r)[0] = (v)[0] * vs + (w)[0] * t;\
	(r)[1] = (v)[1] * vs + (w)[1] * t;\
	(r)[2] = (v)[2] * vs + (w)[2] * t;\
	(r)[3] = (v)[3] * vs + (w)[3] * t;\
}\


void quat_mul(float* r, float* p, float* q)
{	
	float tmp[3];
	cross_prod(r, p, q);
	scl3(tmp, p, q[3]);
	add3(r, r, tmp);
	scl3(tmp, q, p[3]);
	add3(r, r, tmp);

	r[3] = p[3] * q[3] - dot3(p, q);
}

static void quat_from_vec(float* q, float* v)
{
	// TODO
	float ref_dir[] = { 0, 1, 0 };
	float t = acosf(dot3(v, ref_dir));
	float s = sinf(t / 2);
	float a[3];

	cross_prod(a, v, ref_dir);

	q[0] = a[0] * s;
	q[1] = a[1] * s;
	q[2] = a[2] * s;
	q[3] = cosf(t / 2);

	float mag = 1.f / sqrt(dot4(q, q));  // normalize the result of the lerp
	scl4(q, q, mag);               // since it won't lie on the unit sphere.

}	

// current quaternion estimation with inital conditions
float Q[4];
static float vec_quat[4];
static float w_quat[4];
static float d_q_est[4], q_tmp[4];
static float m_mag, a_mag;
static basisf_t basis;

static void quat_ident(float* q)
{
	q[0] = q[1] = q[2] = 0;
	q[3] = 1;
}

void MARG_reset()
{
	quat_ident(Q);
	quat_ident(w_quat);
}

void MARG_tick(float w_x, float w_y, float w_z,
               float a_x, float a_y, float a_z,
               float m_x, float m_y, float m_z,
               float delta_t)
{
	int m[] = { m_x, m_y, m_z }, a[] = { a_x, a_y, a_z };

	// calculate the inverse magnitude of the mag and acc
	// vectors and normalize
	m_mag = 1.f / sqrt(dot3(m, m)); a_mag = 1.f / sqrt(dot3(a, a));
	scl3(basis.up.v, a, a_mag);
	scl3(basis.forward.v, m, m_mag);

	// complete orthogonal basis
	cross_prod(basis.left.v, basis.up.v, basis.forward.v);
	cross_prod(basis.forward.v, basis.left.v, basis.up.v);

	// create orientation quaternion from computed forward vector
	//quat_from_vec(Q, basis.forward.v);

	quat_from_vec(vec_quat, basis.forward.v);

	// compute the derivative of orientation in respect to
	// angular rate of change
	//w_x = w_y = w_z = 0;
	float w[] = { w_x, w_y, w_z, 0.f };

	quat_mul(Q, Q, w);

	scl4(q_tmp, Q, 0.5);
	quat_mul(d_q_est, q_tmp, w);

	// compute the new estimated orientation from angular rate of change
	scl4(d_q_est, d_q_est, delta_t);
	add4(w_quat, Q, d_q_est);
	a_mag = 1.f / sqrt(dot4(w_quat, w_quat));  // normalize the result of the lerp
	scl4(w_quat, w_quat, a_mag);

	// combine both estimates lerped by a convergence value
	const float conv = 0.0f; // Convergence rate, favors w_quat. But should be
	                         // small enough to allow vec_quat to counter-act
				 // the drift of the gyroscope
	lerp4(Q, vec_quat, w_quat, conv);
	a_mag = 1.f / sqrt(dot4(Q, Q));  // normalize the result of the lerp
	scl4(Q, Q, a_mag);               // since it won't lie on the unit sphere.

}
