#ifndef _GPBOTLIB_EVENT_H_
#define _GPBOTLIB_EVENT_H

#include <gpbotlib/string.h>
#include <gpbotlib/result.h>
#include <stdbool.h>

typedef enum {
  GP_EVENT_JOIN,
  GP_EVENT_DISCONNECT,
  GP_EVENT_NONE,
} Gp_Event_Type;

typedef struct {} Gp_Event_None;
typedef struct {} Gp_Event_Join;
typedef struct {
  Gp_String reason;
} Gp_Event_Disconnect;

typedef struct {
  Gp_Event_Type type;
  union Gp_Event_Union {
    Gp_Event_None none;
    Gp_Event_Join join;
    Gp_Event_Disconnect disconnect;
  } event;
} Gp_Event;

typedef struct {
  Gp_Event *allocatedStart;
  uint8_t offset;
  uint16_t used; // this is without counting offset
  uint16_t allocated;
} Gp_Event_Stack;

Gp_Result _gp_init_event_stack(Gp_Event_Stack *stack);
Gp_Result _gp_add_event(Gp_Event_Stack *stack, Gp_Event event);
Gp_Event gp_poll_event(Gp_Event_Stack *stack);
Gp_Result _gp_free_event_stack(Gp_Event_Stack *stack);

#endif // _GPBOTLIB_EVENT_H
