#ifndef   _GPBOTLIB_RESULT_H_
#define   _GPBOTLIB_RESULT_H_

typedef enum Gp_Result {
  GP_VARINT_TOO_BIG = -2,
  GP_INVALID_ARGS = -1,
  GP_SUCCESS = 0,
} Gp_Result;

#endif // _GPBOTLIB_RESULT_H_