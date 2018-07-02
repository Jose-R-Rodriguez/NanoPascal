#!/bin/bash

PC=fpc
PCFLAGS=-MOBJFPC
TINYPASCAL=./bin/nanopascal

mkdir -p bin
COUNT=0
for sfile in $(ls *.pas); do
    BASENAME=$(basename -s.pas $sfile)
    EXFILE=bin/$BASENAME
    INFOFILE=${BASENAME}.out
    echo -n "Testing file $sfile ... "
    $PC $PCFLAGS -o$EXFILE $sfile &> bin/$INFOFILE
    if [ $? -ne 0 ]; then
        echo "Error compiling"
        exit 1
    fi
    $EXFILE > ${BASENAME}.out1
    $TINYPASCAL $sfile > ${BASENAME}.out2
    if [ $? -ne 0 ]; then
        echo "* Failed"
        COUNT=$(( COUNT + 1 ))
    else
        diff ${BASENAME}.out1 ${BASENAME}.out2 &> /dev/null
        if [ $? -ne 0 ]; then
            echo "** Failed"
            COUNT=$(( COUNT + 1 ))
        else
            echo "Passed"
        fi
    fi
done
if [ $COUNT -ne 0 ]; then
    echo "$COUNT test(s) failed."
else
    echo "All tests passed."
fi
rm -f bin/*.o *.out1 *.out2
