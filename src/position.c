#include "gpbotlib/position.h"

Gp_Position gp_position_from_xyz(int32_t x, int32_t y, int32_t z) {
  return (Gp_Position)((Gp_Position)(x & 0x3FFFFFF) << 38) | ((Gp_Position)(z & 0x3FFFFFF) << 12) | (Gp_Position)(y & 0xFFF);
}

void gp_position_to_xyz(Gp_Position position, int32_t *x, int32_t *y, int32_t *z) {
  if (x) {
    *x = (position >> 38) & 0x3FFFFFF;
    if (*x & (1 << 25)) *x |= ~0x3FFFFFF;
  }

  if (z) {
    *z = (position >> 12) & 0x3FFFFFF;
    if (*z & (1 << 25)) *z |= ~0x3FFFFFF;
  }

  if (y) {
    *y = (position >>  0) & 0xFFF;
    if (*y & (1 << 11)) *y |= ~0xFFF;
  }
}