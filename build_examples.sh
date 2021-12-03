#!/bin/sh
for dir in ./examples/*/
do
    dir=${dir%*/}
    pio ci examples/${dir##*/} --board=micro --lib=src
done
