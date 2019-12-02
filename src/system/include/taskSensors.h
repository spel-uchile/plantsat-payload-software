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

void taskSensors(void *param);

#endif //_TASKSENSORS_H
