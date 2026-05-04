cmake_minimum_required(VERSION 3.10)

project(PowerQualityWaveformAnalyser C)

set(CMAKE_C_STANDARD 99)

add_executable(analyser
        main.c
        waveform.c
        io.c
)

target_link_libraries(analyser m)