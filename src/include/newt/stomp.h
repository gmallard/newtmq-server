#ifndef __STOMP_H__
#define __STOMP_H__

#include <newt/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define FNAME_LEN 64
#define LD_MAX (8192)

/* These values specify status of making frame */
#define STATUS_BORN         (1 << 0)
#define STATUS_INPUT_NAME   (1 << 1)
#define STATUS_INPUT_HEADER (1 << 2)
#define STATUS_INPUT_BODY   (1 << 3)
#define STATUS_IN_BUCKET    (1 << 4)
#define STATUS_IN_QUEUE     (1 << 5)

/* These values specify state of connection */
#define STATE_INIT (1 << 0)
#define STATE_CONNECTED (1 << 1)

#define IS_BL(buf) (buf != NULL && buf[0] == '\0')
#define IS_NL(buf) (buf != NULL && (buf[0] == '\n' || buf[0] == '\r'))

typedef struct stomp_conninfo_t stomp_conninfo_t;

typedef struct frame_bucket_t {
  pthread_mutex_t mutex;
  struct list_head h_frame;
} frame_bucket_t;

/* This describes STOMP Frame*/
typedef struct frame_t frame_t;
struct frame_t {
  char name[FNAME_LEN];
  int sock;
  unsigned int status;
  struct list_head h_attrs;
  struct list_head h_data;
  struct list_head l_bucket;
  struct list_head l_transaction;

  /* To know the connection state */
  stomp_conninfo_t *cinfo;

  /* This parameters are used for transaction processing */
  int (*transaction_callback)(frame_t *);
  void *transaction_data;
};

/* This describes a Frame attribute */
typedef struct linedata_t {
  char data[LD_MAX];
  struct list_head l_frame;
} linedata_t;

/* This is alive during connection is active */
struct stomp_conninfo_t {
  int status;
  frame_t *frame;
  char line_buf[LD_MAX];
  char *remained_data;
  int remained_size;
};

typedef struct stomp_header_handler {
  char *name;
  int (*handler)(char *, void *);
} stomp_header_handler_t;

/* These functions are implemented in stomp_driver.c */
int stomp_init();
stomp_conninfo_t *stomp_conn_init();
int stomp_conn_finish(void *);
int stomp_recv_data(char *, int, int, void *);

frame_t *alloc_frame();
void free_frame(frame_t *);

frame_t *get_frame_from_bucket();

/* This is used over each STOMP handlers */
void stomp_send_error(int, char *);
void stomp_send_receipt(int, char *);

#endif
