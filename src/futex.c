#include <sys/queue.h>
#include <parlib/mcs.h>
#include "internal/assert.h"
#include <stdio.h>
#include <errno.h>
#include "lithe.h"
#include "futex.h"

struct futex_element {
  TAILQ_ENTRY(futex_element) link;
  lithe_context_t *context;
  int *uaddr;
};
TAILQ_HEAD(futex_queue, futex_element);

struct futex_data {
  mcs_lock_t lock;
  struct futex_queue queue;
};
static struct futex_data __futex = {
  .lock = MCS_LOCK_INIT,
  .queue = TAILQ_HEAD_INITIALIZER(__futex.queue)
};

static void print_futex_queue()
{
  struct futex_element *e;
  mcs_lock_qnode_t qnode = MCS_QNODE_INIT;
  mcs_lock_lock(&__futex.lock, &qnode);
  printf("FUTEX_QUEUE:\n");
  TAILQ_FOREACH(e, &__futex.queue, link) {
    printf("  %p: (%p, %p)\n", e, e->uaddr, e->context);
  }
  mcs_lock_unlock(&__futex.lock, &qnode);
}

static void __futex_block(lithe_context_t *context, void *arg) {
  struct futex_element *e = (struct futex_element*)arg;
  e->context = context;
}

static inline int futex_wait(int *uaddr, int val)
{
  mcs_lock_qnode_t qnode = MCS_QNODE_INIT;
  mcs_lock_lock(&__futex.lock, &qnode);
  if(*uaddr == val) {
    struct futex_element e;
    e.uaddr = uaddr;
    e.context = NULL;
    TAILQ_INSERT_TAIL(&__futex.queue, &e, link);
    mcs_lock_unlock(&__futex.lock, &qnode);

    lithe_context_block(__futex_block, &e);
  }
  else {
    mcs_lock_unlock(&__futex.lock, &qnode);
  }
  return 0;
}

static inline int futex_wake(int *uaddr, int count)
{
  struct futex_element *e,*n = NULL;
  struct futex_queue q = TAILQ_HEAD_INITIALIZER(q);

  // Atomically grab all relevant futex blockers
  // from the global futex queue
  mcs_lock_qnode_t qnode = MCS_QNODE_INIT;
  mcs_lock_lock(&__futex.lock, &qnode);
  e = TAILQ_FIRST(&__futex.queue);
  while(e != NULL) {
    if(count > 0) {
      n = TAILQ_NEXT(e, link);
      if(e->uaddr == uaddr) {
        TAILQ_REMOVE(&__futex.queue, e, link);
        TAILQ_INSERT_TAIL(&q, e, link);
        count--;
      }
      e = n;
    }
    else break;
  }
  mcs_lock_unlock(&__futex.lock, &qnode);

  // Unblock them outside the lock
  e = TAILQ_FIRST(&q);
  while(e != NULL) {
    n = TAILQ_NEXT(e, link);
    TAILQ_REMOVE(&q, e, link);
    while(e->context == NULL)
      cpu_relax();
    lithe_context_unblock(e->context);
    e = n;
  }
  return 0;
}

int futex(int *uaddr, int op, int val, const struct timespec *timeout,
                 int *uaddr2, int val3)
{
  assert(timeout == NULL);
  assert(uaddr2 == NULL);
  assert(val3 == 0);

  switch(op) {
    case FUTEX_WAIT:
      return futex_wait(uaddr, val);
    case FUTEX_WAKE:
      return futex_wake(uaddr, val);
    default:
      errno = ENOSYS;
      return -1;
  }
  return -1;
}

