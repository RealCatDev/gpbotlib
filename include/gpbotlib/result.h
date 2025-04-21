#ifndef   _GPBOTLIB_RESULT_H_
#define   _GPBOTLIB_RESULT_H_

typedef enum Gp_Result {
  GP_UNDERFLOW = -6,
  GP_BUY_MORE_RAM = -5,
  GP_VARINT_TOO_LONG = -4,
  GP_INVALID_ARGS = -3,
  GP_READ_ERROR = -2,
  GP_WRITE_ERROR = -1,
  GP_SUCCESS = 0,
} Gp_Result;

const char *gp_result_as_cstr(Gp_Result result);

#endif // _GPBOTLIB_RESULT_H_
