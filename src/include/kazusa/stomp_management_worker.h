#ifndef __STOMP_MANAGEMENT_H__
#define __STOMP_MANAGEMENT_H__

#include <kazusa/stomp.h>
#include <pthread.h>

typedef struct subscribe_t {
  char id[LD_MAX];
  pthread_t *thread_id;
  struct list_head list;
} subscribe_t;

typedef struct management_t {
  pthread_mutex_t mutex;

  struct list_head h_subscribe;
} management_t;

void *stomp_management_worker(void *data);
int iterate_header(struct list_head *, stomp_header_handler_t *, void *);

int initialize_manager();
int register_subscriber(char *, pthread_t *);
int unregister_subscriber(char *);
subscribe_t *get_subscriber(char *);

#endif
