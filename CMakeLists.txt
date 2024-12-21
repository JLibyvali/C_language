cmake_minimum_required ( VERSION 3.28 )
project ( C_MORDEN )

set ( GCC_COMPILE_FLAGS "-Wno-incompatible-pointer-types" )

set ( CMAKE_C_STANDARD 11 )
set ( CMAKE_C_STANDARD_REQUIRED ON )
set ( CMAKE_C_FLAGS_DEBUG "-g3 -ggdb3 -O0 -Wall ${GCC_COMPILE_FLAGS}" )
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

function ( more_debug_info _target_name )
    set ( debug_ARCHIVES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${_target_name}.dir )

    target_compile_options ( ${_target_name} PRIVATE -save-temps=obj )
    target_compile_options ( ${_target_name} PRIVATE
        -Wa,-a,-ad > ${debug_ARCHIVES}/${_target_name}.cod
    )
    target_link_options ( ${_target_name} PRIVATE
        -Wl,-Map=${debug_ARCHIVES}/${_target_name}.map
    )

    # Generate disassembler files
    add_custom_command (
        TARGET ${_target_name} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -S --source-comment="[@@@SOURCES@@@]" ${CMAKE_CURRENT_BINARY_DIR}/${_target_name} > ${debug_ARCHIVES}/${_target_name}.disasm
        COMMENT "@@@@@@@@@ Generating Disassembler Information @@@@@@@@@@"
        VERBATIM
    )
endfunction ( more_debug_info _target_name )

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

################ Linux IPC
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

# mmap()
add_executable ( mmap_private Modern_C/IPC/memory/mmap_private.c )
add_executable ( mmap_shared Modern_C/IPC/memory/mmap_shared.c )
add_executable ( mmap_anonymous Modern_C/IPC/memory/mmap_anony.c )
add_executable ( mprotect Modern_C/IPC/memory/mprotect.c )
add_executable ( mlock Modern_C/IPC/memory/mlock.c )
add_executable ( madvise Modern_C/IPC/memory/madvise.c )

# message queue
add_executable(mq_create Modern_C/IPC/msg_queue/mq_create.c)
add_executable(mq_open Modern_C/IPC/msg_queue/mq_open.c)