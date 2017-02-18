#ifndef __MARG
#define __MARG
#include <math.h>

typedef union {
	struct { float x, y, z };
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

float MARG_tick(int w_x, int w_y, int w_z,
                int a_x, int a_y, int a_z,
                int m_x, int m_y, int m_z);

#endif
