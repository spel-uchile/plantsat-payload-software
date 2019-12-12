cmake_minimum_required(VERSION 3.5)
project(SUCHAI_Raspberry_Drivers)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        /home/pi/Spel/chamber_ws/plantsat-payload-software/SUCHAI-Flight-Software/src/drivers/rpi/i2c.c
        lp5562.c
        lp5562_example.c
        )

include_directories(
        /home/pi/Spel/chamber_ws/plantsat-payload-software/SUCHAI-Flight-Software/src/drivers/rpi/include/
        include/
)

link_libraries(-lpthread)

# Use pthread_setname_np included in <features.h>
add_definitions(-D_GNU_SOURCE)

add_executable(lp5562_example ${SOURCE_FILES})
