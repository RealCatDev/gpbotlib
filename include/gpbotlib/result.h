#ifndef   _GPBOTLIB_RESULT_H_
#define   _GPBOTLIB_RESULT_H_

typedef enum Gp_Result {
  GP_VARINT_TOO_BIG = -4,
  GP_INVALID_ARGS = -3,
  GP_READ_ERROR = -2,
  GP_WRITE_ERROR = -1,
  GP_SUCCESS = 0,
} Gp_Result;

#endif // _GPBOTLIB_RESULT_H_
