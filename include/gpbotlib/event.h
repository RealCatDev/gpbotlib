#ifndef _GPBOTLIB_EVENT_H_
#define _GPBOTLIB_EVENT_H

#include <gpbotlib/string.h>
#include <gpbotlib/result.h>
#include <stdbool.h>

typedef enum Gp_Event_Type {
  GP_EVENT_NONE = 0,
  GP_EVENT_JOIN,
  GP_EVENT_DISCONNECT,

  COUNT_GP_EVENT_TYPES
} Gp_Event_Type;

typedef struct Gp_Event_Disconnect {
  Gp_String reason;
} Gp_Event_Disconnect;

typedef struct Gp_Event {
  Gp_Event_Type type;
  union Gp_Event_As {
    Gp_Event_Disconnect disconnect;
  } as;
} Gp_Event;

typedef struct Gp_Event_Queue {
  Gp_Event *allocatedStart;
  uint8_t offset;
  uint16_t used; // this is without counting offset
  uint16_t allocated;
} Gp_Event_Queue;

Gp_Result _gp_init_event_queue(Gp_Event_Queue *queue);
Gp_Result _gp_push_event(Gp_Event_Queue *queue, Gp_Event event);
Gp_Result gp_poll_event(Gp_Event_Queue *queue, Gp_Event *event);
Gp_Result _gp_free_event_queue(Gp_Event_Queue *queue);

#endif // _GPBOTLIB_EVENT_H
