#ifndef __MARG
#define __MARG
#include <math.h>

extern float Q[4];

void MARG_tick(float w_x, float w_y, float w_z,
              float a_x, float a_y, float a_z,
              float m_x, float m_y, float m_z);

#endif
