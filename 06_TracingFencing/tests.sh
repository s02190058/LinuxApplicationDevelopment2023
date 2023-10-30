#!/bin/bash

INFILE="input.txt"
OUTFILE="output.txt"

SUCCESS=0
ARGS_ERROR=1
FOPEN_ERROR=2
FREAD_ERROR=3


./move $INFILE
RET_CODE=$?
if [ $RET_CODE -ne $ARGS_ERROR ]; then
  echo "Invalid arguments test failed."
  exit $RET_CODE
fi

strace -eopenat -e fault=openat:error=EACCES:when=3 ./move $INFILE $OUTFILE
RET_CODE=$?
if [ $RET_CODE -ne $FOPEN_ERROR ]; then
  echo "File open test failed."
  exit $RET_CODE
fi

LD_PRELOAD=`pwd`/myUnlink.so ./move PROTECT_$INFILE $OUTFILE
RET_CODE=$?
if [ $RET_CODE -ne $SUCCESS ];  then
  echo "Custom unlink test failed"
  exit $RET_CODE
fi

echo "Tests passed."