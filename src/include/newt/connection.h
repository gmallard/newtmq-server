#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <newt/config.h>

void *get_connection_info();
void *connection_worker(void *);
int send_msg(int, char *);
int is_socket_valid(int);

#endif
