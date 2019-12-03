/**
 * @file  taskSensors.h
 * @author Carlos Gonzalez C - carlgonz@uchile.cl
 * @date 2019
 * @copyright GNU GPL v3
 *
 * This task implements a listener, that sends commands at periodical times.
 */
#ifndef _TASKSENSORS_H
#define _TASKSENSORS_H

#include "config.h"
#include "globals.h"

#include "osQueue.h"
#include "osDelay.h"

#include "repoCommand.h"


typedef enum machine_action {
    ACT_STAND_BY = 0,
    ACT_START,
    ACT_PAUSE,
    ACT_LAST
} machine_action_t;

typedef enum machine_state {
    ST_PAUSE = 0,
    ST_SAMPLING,
    ST_LAST
} machine_state_t;

typedef struct sample_machine{
    machine_state_t state;
    machine_action_t action;
    payload_id_t payload;
    unsigned int step;
    int samples_left;
} sample_machine_t;

void taskSensors(void *param);

#endif //_TASKSENSORS_H
