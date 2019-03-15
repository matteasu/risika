cmake_minimum_required(VERSION 3.13)
project(risika C)

set(CMAKE_C_STANDARD 90)

add_executable(risika main.c libRisika.c libRisika.h docente.c docente.h libPrep.c libPrep.h libGioco.c libGioco.h libFile.c libFile.h)