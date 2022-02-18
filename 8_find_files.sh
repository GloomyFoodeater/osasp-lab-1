#!/bin/bash
start=$1
end=$2
dir=$3

# Check arguments number
if [ $# -ge 3 ]
then
	# Check directory existence
	if [ -d ".$dir" ]
	then
		echo "Files with sizes in ($start bytes; $end bytes): "
		
		# c stands for bytes in find comand
		start+=c
		end+=c
		
		# %s - file size
		# %f - file name
		find ".$dir" -size +$start -size -$end -printf "%s\t$dir/%f\n" | sort -n
	else
		echo "Error: .$dir didn't exist." >&2
	fi
else
	echo "Error: Unexpected number of parameters($#)
	      1st parameter - start of range in bytes;
	      2nd parameter - end of range in bytes;
	      3rd parameter - directory." >&2
fi
