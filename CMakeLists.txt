cmake_minimum_required(VERSION 3.10)

project(Flair VERSION 1.0 LANGUAGES C)

set(CURSES_INCLUDE_DIR "/clang64/include")
set(CURSES_LIBRARIES "/clang64/bin/libncurses.a")

find_package(Curses 
    REQUIRED)


message("${CURSES_LIBRARIES}")
message("${CURSES_INCLUDE_DIR}")

add_executable(Flair
    src/Editor.c
    src/linkedlist/LinkedList.c
    src/openedFile/openedFile.c
    src/languages/FileText.c
    src/languages/languageC.c
)

target_include_directories(Flair
    PUBLIC
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   
        "/clang64/include" 
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

