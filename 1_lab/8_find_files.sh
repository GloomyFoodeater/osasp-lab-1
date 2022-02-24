#!/bin/bash
start=$1
end=$2
dir=$3

# Check number of args
if [ $# -lt 3 ]
then
	echo "Error: Unexpected number of argumets($#)." >&2
	exit 1
fi

# Check 1st arg
if echo $start | grep -qE "[^0-9]+"
then
	echo "Error: 1st argument($start) is NaN." >&2
	exit 1
fi

# Check 2nd arg
if echo $end | grep -qE "[^0-9]+"
then
	echo "Error: 2nd argument($end) is NaN." >&2
	exit 1
fi

# Check 3rd arg
if [ ! -d $dir ]
then
	echo "Error: Directory $dir didn't exist." >&2
	exit 1
fi

# Program output
echo "Files with sizes in ($start bytes; $end bytes): "

# c stands for bytes in find comand
start+=c
end+=c

# %s - file size
# %p - file path
find $dir -size +$start -size -$end -printf "%s\t%p\n" | sort -n
