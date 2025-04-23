#include <gpbotlib/result.h>
#include <gpbotlib/event.h>

#include <stdlib.h>
#include <string.h>

#define GP_EVENT_QUEUE_CHUNK_SIZE 256

Gp_Result _gp_init_event_queue(Gp_Event_Queue *queue) {
  if (!queue) return GP_INVALID_ARGS;

  queue->allocatedStart = malloc(GP_EVENT_QUEUE_CHUNK_SIZE * sizeof(Gp_Event));
  if (!queue->allocatedStart) return GP_BUY_MORE_RAM;

  queue->offset = 0;
  queue->used = 0;
  queue->allocated = GP_EVENT_QUEUE_CHUNK_SIZE;

  return GP_SUCCESS;
}

Gp_Result _gp_push_event(Gp_Event_Queue *queue, Gp_Event event) {
  if (!queue) return GP_INVALID_ARGS;
  if (!queue->used) queue->offset = 0;

  if (queue->used + queue->offset >= queue->allocated) {
    queue->allocated += GP_EVENT_QUEUE_CHUNK_SIZE;
    queue->allocatedStart = realloc(queue->allocatedStart, queue->allocated * sizeof(Gp_Event));
    if (!queue->allocatedStart) return GP_BUY_MORE_RAM;
  }

  queue->allocatedStart[queue->used++ + queue->offset] = event;

  return GP_SUCCESS;
}

Gp_Result gp_poll_event(Gp_Event_Queue *queue, Gp_Event *event) {
  if (!queue) return GP_INVALID_ARGS;
  if (!queue->used) return GP_SUCCESS;
  if (event) *event = queue->allocatedStart[--queue->used, queue->offset++];

  return GP_TRUE;
}

Gp_Result _gp_free_event_queue(Gp_Event_Queue *queue) {
  if (!queue) return GP_INVALID_ARGS;
  free(queue->allocatedStart);
  return (queue->allocatedStart = NULL, GP_SUCCESS);
}