cmake_minimum_required(VERSION 3.23)
project(Dwm)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(L_Xinerama INTERFACE)
target_link_libraries(L_Xinerama INTERFACE Xinerama)
target_compile_definitions(L_Xinerama INTERFACE XINERAMA)

add_library(L_FreeType2 INTERFACE)
target_include_directories(L_FreeType2 INTERFACE /usr/include/freetype2)
target_link_libraries(L_FreeType2 INTERFACE
                        fontconfig
                        Xft)

add_library(L_X11 INTERFACE)
target_include_directories(L_X11 INTERFACE /usr/X11R6/include)
target_link_directories(L_X11 INTERFACE /usr/X11R6/lib $HOME/meow)
target_link_libraries(L_X11 INTERFACE X11 L_FreeType2)

add_executable(dwm drw.c dwm.c util.c autostart.h config.h myapps.h)
set(VERSION 3.6)
target_compile_options(dwm PUBLIC
                        -pedantic
                        -Wall
                        -Wno-deprecated-declarations)
target_compile_definitions(dwm PUBLIC
                            _DEFAULT_SOURCE
                            _BSD_SOURCE
                            _POSIX_C_SOURCE=200809L
                            VERSION=\"${VERSION}\")
target_link_libraries(dwm L_X11 L_Xinerama)

add_custom_command(
    OUTPUT autostart.h config.h myapps.h
    COMMAND cp -n ${PROJECT_SOURCE_DIR}/autostart.def.h ${PROJECT_SOURCE_DIR}/autostart.h
    COMMAND cp -n ${PROJECT_SOURCE_DIR}/config.def.h ${PROJECT_SOURCE_DIR}/config.h
    COMMAND cp -n ${PROJECT_SOURCE_DIR}/myapps.def.h ${PROJECT_SOURCE_DIR}/myapps.h
)

install(TARGETS dwm DESTINATION ~/.local/bin)