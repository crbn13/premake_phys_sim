#!/bin/bash

# This script is used because im lazy and cannot be asked to dive down the directory tree every time i want to run the program

echo " By default will run the DEBUG app, but command argument can be used instead : debug release "

type="debug"

if [ "$1" == "debug" ]; then
  type="debug"
fi
if [ "$1" == "release" ]; then
  type="release"
fi

echo "Running $type : "
echo ""

dir="binaries/"$type"/app/"
cd $dir
./app
