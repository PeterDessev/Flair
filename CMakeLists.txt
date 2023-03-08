cmake_minimum_required(VERSION 3.10)

project(Flair VERSION 1.0 LANGUAGES C)

find_package(Curses 
    REQUIRED)

# add_library(CURSES SHARED IMPORTED)
# set_target_properties(CURSES PROPERTIES IMPORTED_LOCATION /clang64/lib/lncurses.a)
# set(CURSES_INCLUDE_DIR /clang64/include)

message(STATUS "${CURSES_LIBRARIES}")
message(STATUS "${CURSES_INCLUDE_DIR}")

add_executable(Flair
    src/Editor.c
    src/linkedlist/LinkedList.c
    src/openedFile/openedFile.c
    src/types/FileText.c
    src/types/languageC.c
)

target_include_directories(Flair
    PUBLIC
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   
        ${CURSES_INCLUDE_DIR}   
)

target_link_libraries(Flair
    PRIVATE
        ${CURSES_LIBRARIES}
)

target_compile_options(Flair
    PRIVATE
        -Wall
)

target_compile_definitions(Flair
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

