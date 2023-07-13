#!/bin/bash

function dzwigi_start(){
  echo "Nadzorca zaczal prace"
  ~/SO/LAB7_procesy/dzwig1.sh &
  pid1=$!
  echo "Dzwig 1 zaczal prace w tle"
  ~/SO/LAB7_procesy/dzwig2.sh &
  pid2=$!
  echo "Dzwig 2 zaczal prace w tle"
  wait $pid1
  files_moved=$?
  echo "Dzwig 1 zakonczyl prace"
  wait $pid2
  echo "Dzwig 2 zakonczyl prace"
  echo "Files moved: $files_moved"
  dzwigi_koniec
}

trap dzwigi_start USR1

function dzwigi_koniec(){
  echo "Nadzorca konczy prace"
  exit 0
}

trap dzwigi_koniec SIGINT

while :
do
  echo -e "Sygnal do rozpoczecia: kill -USR1 $$\nSygnal do zakonczenia: ctrl^c\n"
  sleep 15 
done

