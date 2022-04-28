#!/bin/bash

#args=$($1 $2 $3)
echo "setting barcode environmetal variable values"
export BARCODE=false
export DIMENSIONS=true
export DEBUG_DIMENSIONS=false
echo "Perform Operation "
./agv --showdata --debug 2 --showimage  
