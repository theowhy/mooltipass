# this one is important
set(CMAKE_SYSTEM_NAME Generic)
#this one not so much
set(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
set(CMAKE_C_COMPILER   /usr/bin/avr-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/avr-g++)

# disable search for programs on host
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
