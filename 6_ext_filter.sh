#!/bin/bash

# Obtain arguments
dst_file_name=$1
src_dir_name=$2
extension=$3

# Check number of arguments
if [ $# -ge 3 ]
then
	# Check correctness of arguments
	if [ -d $src_dir_name ]
	then
		# Output found file names to destination file
		curr_dir=$(pwd) # Remember current directory
		cd $src_dir_name
		ls *.$extension | sort -d  >$curr_dir/$dst_file_name
		echo "Success! Check $curr_dir/$dst_file_name to see the output."
	else
		# Output error
		echo "Error: $src_dir_name is not a directory." >&2
	fi
else
	# Output help
	echo "Error: Unexpected number of parameters($#).
	      1st parameter - name of destination file to write found files;
	      2nd parameter - name of directory to search through;
	      3rd parameter - extension of files to find." >&2
fi

