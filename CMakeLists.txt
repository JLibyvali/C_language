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
add_executable ( c_oop
    Modern_C/oop/dictionary.c
    Modern_C/oop/keyval.c
    Modern_C/oop/point.c
    Modern_C/oop/vtable.c
    Modern_C/oop/private.c
    Modern_C/oop/user.c
)
target_compile_options ( c_oop PRIVATE
    -fms-extensions
)
target_link_options ( c_oop PRIVATE
    -lm )

add_executable ( c_reference_count Modern_C/oop/refer.c )
add_executable ( c_overlaod Modern_C/oop/overload.c )

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