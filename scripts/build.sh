#!/bin/bash

PROJECT_NAME="Tic-Tac-Toe"

echo "Generating makefiles..."
./vendor/premake/premake5 gmake2

echo "Building the project in Debug mode..."
cd build || exit
make

cd ..
echo "Running the executable..."
./bin/Debug/$PROJECT_NAME