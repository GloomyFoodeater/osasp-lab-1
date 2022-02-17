#!/bin/bash
src=$1
dst=$2

# Check number of arguments
if [ $# -ge 2 ]
then
	
	# Check file existence
	if [ -f $src ]
	then
		gcc $src -o $dst.exe
		
		# Check previous command code
		if [ $? -ne 0 ]
		then
			echo "Error: Compilation failed!" >&2
		else
			echo "Compilation succeded!"
		fi
	else
		echo "Error: File didn't exist!" >&2
	fi
else
	echo "Error: Not enough aruments" >&2
fi
