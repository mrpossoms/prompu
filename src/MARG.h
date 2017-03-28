#ifndef __MARG
#define __MARG
#include <math.h>

typedef union {
	struct { float x, y, z; };
	float v[3];
} vec3f_t;

typedef struct {
	vec3f_t left, forward, up;
} basisf_t;

/*
typedef struct {
	struct { int x, y, z } left, forward, up;
} basisi_t;
*/

extern float Q[4];

void MARG_reset();

void MARG_tick(float w_x, float w_y, float w_z,
               float a_x, float a_y, float a_z,
               float m_x, float m_y, float m_z,
               float delta_t);

#endif
