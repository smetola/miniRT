#!/bin/bash

scenes="$(find scenes/error -type f -name '*.rt')"
for scene in ${scenes}; do
    valgrind --error-exitcode=2 ./miniRT "$scene" &>temp
    status=$?
    if [ $status -eq 2 ]; then
        echo "Error in scene $scene:"
        cat temp
        break
    fi
done;
rm temp