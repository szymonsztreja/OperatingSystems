#!/bin/bash

files_moved=0

for f in ~/SO/LAB7_procesy/pierwszy/*
do
	while [ $(ls bufor/ | wc -l) -ge 3 ]
	do
	sleep 1
	done
	mv "$f" bufor/ 
	((files_moved++))
	echo "Dzwig1 przeniosl plik: $f"
done
exit $files_moved


