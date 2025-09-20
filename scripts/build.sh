#!/bin/bash

PROJECT_NAME="Tic-Tac-Toe"
CONFIG="Debug"

ConfigProject() {
    ./vendor/premake/premake5 clean
    echo "==== Generating ($CONFIG) Build Files ===="
    ./vendor/premake/premake5 gmake2
    echo "==== Build Files Generation Complete ===="
    echo
}

BuildProject(){
    echo "==== BUILDING PROJECT $PROJECT_NAME ===="
    cd "$PROJECT_NAME/" && bear -- make
    echo "==== BUILD FINISHED ===="
    cd ..
    echo
}

RunProject(){
    echo "==== Running the executable:$PROJECT_NAME.out ===="
    exedir=$(find . -name "$PROJECT_NAME.out")
    if [ -z "$exedir" ]
    then
        echo "Executable $PROJECT_NAME.out not found"
        echo "Project Not Compiled"
        echo "==== ===="
    else
        $exedir
    echo "========"
    echo
    fi
}

if [[ $# = 0 ]]; then
    ConfigProject
    BuildProject
    RunProject
elif [[ "$1" = "-b" ]]; then
    BuildProject
elif [[ "$1" = "-c" ]]; then
    ConfigProject
fi
