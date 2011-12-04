project(ecg)
cmake_minimum_required(VERSION 2.8)

find_package(GLUT)
find_package(OpenGL)

set(CMAKE_C_FLAGS "-Wall -std=c99")

add_executable(ECG MACOSX_BUNDLE WIN32
    main.c
    graphics.c
    audio.c
    analyze.c
)

target_link_libraries(ECG portaudio ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
