#!/bin/bash

MYDIR=$(dirname $0)
LD_LIBRARY_PATH=$MYDIR/libs:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

exec ${MYDIR}/vokoscreenNG

