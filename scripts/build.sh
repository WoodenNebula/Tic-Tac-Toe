#!/bin/bash

PROJECT_NAME="Tic-Tac-Toe"
CONFIG="Debug"

BuildProject(){
    ./vendor/premake/premake5 clean
    echo "==== Building Process Started ($CONFIG) ===="
    echo "Generating makefiles..."
    ./vendor/premake/premake5 gmake2
    cd "$PROJECT_NAME/" && make
    echo "==== BUILD FINISHED ===="
    echo
}

RunProject(){
    echo "==== Running the executable ===="
    exedir=$(find -name "$PROJECT_NAME.out")
    if [ -z "$exedir" ]
    then
        echo "$exedir doesnt exist"
        echo "$PROJECT_NAME.out not found"
        echo "Project Not Compiled"
        echo "==== ===="
    else
        $exedir
    echo "==== ===="
    echo
    fi
}

BuildProject
RunProject