#!/bin/bash

# du - disk usage  -s summarize and cut leaves only the first passed field
dir_size=$(du -k ~/backups | cut -f1)

# Maximum size of a directory in kilobytes
max_size=2
if [ $dir_size -ge $max_size ]
then
	echo -e "Maximum directory size reached!\nMaximum size: "$max_size"\nCurrent size: "$dir_size 
fi

exit 0
