#!/bin/bash 

begintime="2018-01-01"

echo "Commits seit dem:  "$begintime

echo "Anzahl commits:    "$(git log --shortstat --since="$begintime" | grep commit | wc -l)

echo "Eingefügte Zeilen: "$(git log --shortstat --since="$begintime" | grep insertions | cut -d "," -f2 | sed 's/^[ \t]*//' | cut -d" " -f1 | paste -sd+ | tr -s "+" | bc)
echo "Gelöschte Zeilen:  "$(git log --shortstat --since="$begintime" | grep deletions  | cut -d "," -f3 | sed 's/^[ \t]*//' | cut -d" " -f1 | paste -sd+ | tr -s "+" | bc)

