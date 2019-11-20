cmake_minimum_required(VERSION 3.5)
project(PlantSat-Payload)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        src/drivers/mcp9808.c
        src/system/taskInit.c
        src/system/cmdSensors.c
        SUCHAI-Flight-Software/src/system/main.c
        SUCHAI-Flight-Software/src/drivers/rpi/init.c
        SUCHAI-Flight-Software/src/drivers/rpi/data_storage.c
        SUCHAI-Flight-Software/src/drivers/rpi/i2c.c
        SUCHAI-Flight-Software/src/os/Linux/osDelay.c
        SUCHAI-Flight-Software/src/os/Linux/osQueue.c
        SUCHAI-Flight-Software/src/os/Linux/osScheduler.c
        SUCHAI-Flight-Software/src/os/Linux/osSemphr.c
        SUCHAI-Flight-Software/src/os/Linux/osThread.c
        SUCHAI-Flight-Software/src/os/Linux/pthread_queue.c
        SUCHAI-Flight-Software/src/system/cmdDRP.c
        SUCHAI-Flight-Software/src/system/cmdOBC.c
        SUCHAI-Flight-Software/src/system/cmdCOM.c
        SUCHAI-Flight-Software/src/system/cmdFP.c
        SUCHAI-Flight-Software/src/system/cmdTM.c
        SUCHAI-Flight-Software/src/system/cmdConsole.c
        SUCHAI-Flight-Software/src/system/repoCommand.c
        SUCHAI-Flight-Software/src/system/repoData.c
        SUCHAI-Flight-Software/src/system/taskDispatcher.c
        SUCHAI-Flight-Software/src/system/taskExecuter.c
        SUCHAI-Flight-Software/src/system/taskCommunications.c
        SUCHAI-Flight-Software/src/system/taskConsole.c
        SUCHAI-Flight-Software/src/system/taskFlightPlan.c
        #SUCHAI-Flight-Software/src/system/taskHousekeeping.c
        #SUCHAI-Flight-Software/src/system/taskInit.c
        SUCHAI-Flight-Software/src/system/taskWatchdog.c
)

include_directories(
        src/drivers/include
        src/system/include
        SUCHAI-Flight-Software/src/system/include/
        SUCHAI-Flight-Software/src/os/include
        SUCHAI-Flight-Software/src/drivers/rpi/include
        SUCHAI-Flight-Software/src/drivers/rpi/libcsp/include
        /usr/include/postgresql
)

link_directories(
        SUCHAI-Flight-Software/src/drivers/rpi/libcsp/lib
)

link_libraries(-lpthread -lsqlite3 -lpq -lzmq -lcsp)

# Use pthread_setname_np included in <features.h>
add_definitions(-D_GNU_SOURCE)

add_executable(PlantSat-Payload ${SOURCE_FILES})

