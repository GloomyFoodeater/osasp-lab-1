#!/bin/bash
src=$1
dst=$2

# Check number of arguments
if [ $# -ge 2 ]
then
	
	# Check file existence
	if [ -f $src ]
	then
		gcc $src -o $dst.exe && ./$dst.exe
	else
		echo "Error: File didn't exist!" >&2
	fi
else
	echo "Error: Not enough aruments" >&2
fi
