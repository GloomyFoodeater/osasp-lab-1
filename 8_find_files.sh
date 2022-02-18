#!/bin/bash
start=$1
end=$2
dir=".$3"

# Check directory existence
if [ -d $dir ]
then
	echo "Files with sizes in ($start bytes; $end bytes): "
	
	# c stands for bytes in find comand
	start+=c
	end+=c
	
	# %s - file size
	# %p - file name
	find $dir -size +$start -size -$end -printf "%s\t%p\n" | sort -n
else
	echo "Error: $dir didn't exist." >&2
fi

