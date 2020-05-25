#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "event2/event.h"

struct event_args
{
    struct timeval tv;
    struct event *ev;
    int (*fn)(void *);
    void *msg;
};

struct event_base *sg_current_base;

char *get_current_time(void)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return asctime(tm);
}

void sg_event_handler(int fd, short event, void *arg)
{
    struct event_args *evargs = (struct event_args *)arg;
    if (!evargs)
        return;

    if (evargs->fn)
        evargs->fn(evargs->msg);

    evtimer_del(evargs->ev);

    if (evargs)
        free(evargs);
}

int event_timer_handler(void *msg)
{
    printf("time: %s\n", get_current_time());
    return 0;
}

int add_event_timer(unsigned int second, int (*fn)(void *), void *msg)
{
    struct timeval tv = {second, 0};

    struct event_args *args = malloc(sizeof(struct event_args));
    if (!args)
        return -1;

    struct event *ev = evtimer_new(sg_current_base, sg_event_handler, (void *)args);

    args->tv = tv;
    args->ev = ev;
    args->fn = fn;
    args->msg = msg;

    evtimer_add(ev, &tv);

    return 0;
}

int main(int argc, char const *argv[])
{

    sg_current_base = event_init();

    add_event_timer(2, event_timer_handler, NULL);
    add_event_timer(3, event_timer_handler, NULL);
    add_event_timer(4, event_timer_handler, NULL);
    add_event_timer(5, event_timer_handler, NULL);

    event_dispatch();

    printf("event end\n");
    /* code */
    return 0;
}

