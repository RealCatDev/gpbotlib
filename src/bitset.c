#include "gpbotlib/bitset.h"

#include <stdlib.h>

Gp_Result gp_bitset_get(const Gp_Bitset *bitset, size_t n, bool *v) {
  if (!bitset || !v) return GP_INVALID_ARGS;
  if ((n / 64) + 1 > bitset->length) *v = false;
  *v = bitset->data[n / 64] & (n % 64);

  return GP_SUCCESS;
}

Gp_Result gp_bitset_set(Gp_Bitset *bitset, size_t n, bool v) {
  if (!bitset) return GP_INVALID_ARGS;

  if ((n / 64) + 1 > bitset->length) {
    bitset->length = (n / 64) + 1;
    bitset->data = realloc(bitset->data, sizeof(*bitset->data) * bitset->length);
    if (!bitset->data) return GP_BUY_MORE_RAM;
  }

  if (!bitset->data) return GP_INVALID_ARGS;

  uint64_t bit = n % 64;
  if (v) bitset->data[n / 64] |= bit;
  else bitset->data[n / 64] &= ~bit;

  return GP_SUCCESS;
}