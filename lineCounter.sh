#!/bin/bash

cpp=$( find . -name '*.cpp' -type f -print0 | xargs -0 cat | wc -l )
echo "*.cpp:" $cpp

h=$( find . -name '*.h' -type f -print0 | xargs -0 cat | wc -l )
echo "*.h  :" $h

ts=$( find . -name '*.ts' -type f -print0 | xargs -0 cat | wc -l )
echo "*.ts :" $ts

