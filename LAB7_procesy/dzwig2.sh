#!/bin/bash

files_moved=0

while [ $(ls pierwszy/ | wc -l) -gt 0 ] || [ $(ls bufor/ | wc -l) -gt 0 ] 
do
if [ -n "$(ls -A bufor)" ]; then
	for f in bufor/*
	do
	 	mv "$f" drugi 
        	((files_moved++))
	        echo "Dzwig2 przeniosl plik: $f"
	done
	sleep 1
fi
done
exit 0
