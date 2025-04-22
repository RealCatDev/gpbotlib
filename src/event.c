#include <gpbotlib/result.h>
#include <gpbotlib/event.h>

#include <stdlib.h>
#include <string.h>

#define GP_EVENT_STACK_CHUNK_SIZE 256

Gp_Result _gp_init_event_stack(Gp_Event_Stack *stack) {
  stack->allocatedStart = malloc(GP_EVENT_STACK_CHUNK_SIZE * sizeof(Gp_Event));
  stack->offset = 0;
  stack->used = 0;
  stack->allocated = GP_EVENT_STACK_CHUNK_SIZE;
  
  return GP_SUCCESS;
}

Gp_Result _gp_add_event(Gp_Event_Stack *stack, Gp_Event event) {
  if(stack->used + stack->offset >= stack->allocated) {
    Gp_Event *resized = realloc(stack->allocatedStart, (stack->allocated + GP_EVENT_STACK_CHUNK_SIZE) * sizeof(Gp_Event));
    if (!resized)
      return GP_BUY_MORE_RAM;
    stack->allocatedStart = resized;
    stack->allocated += GP_EVENT_STACK_CHUNK_SIZE;
  }
  stack->allocatedStart[(stack->used++) + stack->offset] = event;
}

Gp_Result gp_poll_event(Gp_Event_Stack *stack, Gp_Event *event) {  
  if (stack->used == 0) {
    *event = (Gp_Event){
      .type = GP_EVENT_NONE,
      .event = (Gp_Event_None){}
    };
    return GP_SUCCESS;
  }

  *event  = stack->allocatedStart[stack->offset++];
  if(stack->offset == GP_EVENT_STACK_CHUNK_SIZE) {
    memmove(stack->allocatedStart, stack->allocatedStart + stack->offset * sizeof(Gp_Event), stack->used * sizeof(Gp_Event));
    Gp_Event *resized = realloc(stack->allocatedStart, (stack->allocated - stack->offset));
    if(!resized)
      return GP_BUY_MORE_RAM; // Or less :/
    stack->allocatedStart = resized;
  }
  
  return GP_SUCCESS;
}

Gp_Result _gp_free_event_stack(Gp_Event_Stack *stack) {
  free(stack->allocatedStart);
  stack->allocatedStart == NULL;
}
