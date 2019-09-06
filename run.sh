#!/bin/bash

if [[ "$ command -v asm6f" != "" ]] ; then
    cd ./UNRELATED
    asm6f main.asm game.nes

else
    echo "Please, install asm6f!"
    exit 1
fi

if [[ "$ command -v mednafen" != "" ]] ; then
    mednafen game.nes

else
    echo "Please, install mednafen!"
    exit 1
fi

