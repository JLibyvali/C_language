cmake_minimum_required ( VERSION 3.28 )
project ( C_MORDEN )

set ( CMAKE_C_STANDARD 11 )
set ( CMAKE_C_STANDARD_REQUIRED ON )
set ( CMAKE_C_FLAGS_DEBUG "-g3 -ggdb3 -O0 -Wall" )
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

include_directories (
    ./include
)

# C-style OOP programming
add_executable ( c_oop )
target_sources ( c_oop PRIVATE
    Modern_C/oop/dictionary.c
    Modern_C/oop/keyval.c
    Modern_C/oop/user.c
)

# Linux IPC
add_executable ( bio_pipe
    Modern_C/IPC/pipe/bio_pipe.c
)
add_executable ( pipe_syn
    Modern_C/IPC/pipe/pipe_syn.c
)

add_executable ( fifo_server
    Modern_C/IPC/pipe/fifo_msg.h
    Modern_C/IPC/pipe/fifo_server.c
)
add_executable ( fifo_client
    Modern_C/IPC/pipe/fifo_msg.h
    Modern_C/IPC/pipe/fifo_client.c
)