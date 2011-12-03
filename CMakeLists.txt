project(ecg)
cmake_minimum_required(VERSION 2.8)

find_package(GLUT)
find_package(OpenGL)

set(CMAKE_C_FLAGS "-Wall -std=c99")

set(ECG_NAME ecg)

if(APPLE)
    set(ECG_NAME ECG)
    set(EXECUTABLE_TYPE MACOSX_BUNDLE)
endif(APPLE)

if(WIN32)
    set(ECG_NAME ECG)
    set(EXECUTABLE_TYPE WIN32)
endif(WIN32)

add_executable(${ECG_NAME} ${EXECUTABLE_TYPE}
    main.c
    graphics.c
    audio.c
    analyze.c
)

target_link_libraries(${ECG_NAME} portaudio ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
