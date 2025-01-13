#!/bin/bash

PROJECT_NAME="Tic-Tac-Toe"
CONFIG="Debug"

echo "Generating makefiles..."
./vendor/premake/premake5 gmake2

echo "Building the project in $CONFIG mode..."
cd build || exit
make

cd ..
echo "Running the executable..."
./bin/$CONFIG/$PROJECT_NAME