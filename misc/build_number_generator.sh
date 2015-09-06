#!/bin/bash
number=`cat build_number.txt`
number=`expr $number + 1` 
echo "$number" | tee build_number.txt
echo "#define BUILD ""$number" | tee ../nephele/build_number.h

