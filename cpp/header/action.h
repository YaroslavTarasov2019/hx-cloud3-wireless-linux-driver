#ifndef ACTION_H
#define ACTION_H

#include <ncurses.h>
#include "api.h"

void change_time_to_auto_shutdown_local(int minutes);

void change_mic_monitoring_local();

void recording();

int action(int ch);


#endif
