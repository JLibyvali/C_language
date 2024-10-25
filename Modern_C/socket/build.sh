#!/bin/bash

if [ -d BUILD ]; then
    rm -rf ./BUILD
fi

bear -- make all -j 8