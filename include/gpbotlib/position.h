#ifndef   _GPBOTLIB_POSITION_H_
#define   _GPBOTLIB_POSITION_H_

#include <gpbotlib/result.h>

#include <stdint.h>

typedef uint64_t Gp_Position;

Gp_Position gp_position_from_xyz(int32_t x, int32_t y, int32_t z);
void gp_position_to_xyz(Gp_Position position, int32_t *x, int32_t *y, int32_t *z);

#endif // _GPBOTLIB_POSITION_H_