cmake_minimum_required ( VERSION 3.28 )
project ( C_MORDEN )

include ( cmake/api.cmake )

set ( GCC_COMPILE_FLAGS "-Wno-incompatible-pointer-types" )

set ( CMAKE_C_STANDARD 11 )
set ( CMAKE_C_STANDARD_REQUIRED ON )
set ( CMAKE_C_FLAGS_DEBUG "-g3 -ggdb3 -O0 -Wall ${GCC_COMPILE_FLAGS}" )
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

include_directories (
    ./include
)

# ####################################################################################
# TARGETS
# ####################################################################################

# C-style OOP programming
add_executable ( c_oop
    Modern_C/oop/dictionary.c
    Modern_C/oop/keyval.c
    Modern_C/oop/point.c
    Modern_C/oop/vtable.c
    Modern_C/oop/private.c
    Modern_C/oop/user.c
)
target_compile_options ( c_oop PRIVATE -fms-extensions )
target_link_options ( c_oop PRIVATE -lm )

add_executable ( c_reference_count Modern_C/oop/refer.c )

add_executable ( c_overlaod Modern_C/oop/overload.c )
add_executable ( c_reflection Modern_C/oop/reflection.c )
more_debug_info ( c_reflection )

# ############### Linux IPC
# pipe and fifo
add_executable ( bio_pipe Modern_C/IPC/pipe/bio_pipe.c )
add_executable ( pipe_syn Modern_C/IPC/pipe/pipe_syn.c )

add_executable ( fifo_server
    Modern_C/IPC/pipe/fifo_msg.h
    Modern_C/IPC/pipe/fifo_server.c
)
add_executable ( fifo_client
    Modern_C/IPC/pipe/fifo_msg.h
    Modern_C/IPC/pipe/fifo_client.c
)

# file map
generate_executables ( ${CMAKE_CURRENT_SOURCE_DIR}/Modern_C/IPC/file_map )

# message queue
generate_executables ( ${CMAKE_CURRENT_SOURCE_DIR}/Modern_C/IPC/msg_queue )

# Semphore
generate_executables ( ${CMAKE_CURRENT_SOURCE_DIR}/Modern_C/IPC/semaphore )

# Shared memory
generate_executables ( ${CMAKE_CURRENT_SOURCE_DIR}/Modern_C/IPC/shared_memory )

# File lock
generate_executables ( ${CMAKE_CURRENT_SOURCE_DIR}/Modern_C/IPC/filelock )