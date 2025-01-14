#include "unit.h"

#include <newt/common.h>
#include <newt/daemon.h>

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUM 12346

static void init_newt_config(newt_config *config) {
  config->port = PORT_NUM;
}

static void check_daemon(void) {
  pid_t pid;
  newt_config config;

  init_newt_config(&config);

  if((pid = fork()) == 0) {
    CU_ASSERT(daemon_initialize() == RET_SUCCESS);

    daemon_start(config);
  } else {
    /* delay to wait for initialization of daemon to complete */
    sleep(1);

    kill(pid, SIGINT);

    wait(NULL);
  }
}

int test_daemon(CU_pSuite suite) {
  suite = CU_add_suite("newtd daemon test", NULL, NULL);
  if(suite == NULL) {
    return CU_ERROR;
  }

  CU_add_test(suite, "test daemon_processing", check_daemon);

  return CU_SUCCESS;
}
