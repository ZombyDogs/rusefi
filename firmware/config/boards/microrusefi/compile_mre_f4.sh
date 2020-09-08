#!/bin/bash

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=mre_f4"

# export DEBUG_LEVEL_OPT="-O0 -ggdb -g3"

bash config/boards/common_make.sh