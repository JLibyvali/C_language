cmake_minimum_required ( VERSION 3.28 )

set ( SOURCES ./prog.c )

add_executable ( ANSI_C ${SOURCES} )
target_include_directories (
    ANSI_C
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# linking math library
target_link_libraries ( ANSI_C m )
