#!/bin/bash

#File name
date_now=$(date +'%d-%m-%y')
user=$(whoami)
file_name=$user"_backup_"$date_now".tar.gz"

#Destination make dir only if doesn't exists
dest=~/backups 
mkdir -p  $dest

# Create an empty tar file
touch $dest/$file_name

# Find .txt files in ~/Documents directory. print0 adds null character instead of new line char.
# This makes things safer if we are using other programs like tar.
cd $dest 
find ~/Documents -type f -name "*.txt" | xargs -d '\n' tar czf "$dest/$file_name"
#find ~/Documents -name "*.txt" -type f -exec tar rvf $dest/$file_name {} + 

exit 0
