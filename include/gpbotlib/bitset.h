#ifndef   _GPBOTLIB_BITSET_H_
#define   _GPBOTLIB_BITSET_H_

#include "gpbotlib/varint.h"

#include <stdbool.h>

typedef struct Gp_Bitset {
  Gp_Varint length;
  uint64_t *data;
} Gp_Bitset;

Gp_Result gp_bitset_get(const Gp_Bitset *bitset, size_t n, bool *v);
Gp_Result gp_bitset_set(Gp_Bitset *bitset, size_t n, bool v);

#endif // _GPBOTLIB_BITSET_H_