#!/bin/bash
# dvdcopy.sh - A sample shell script to display a progress bar
# set counter to 0 

# set infinite while loop
while :
do
sleep 1
folder=$(ls out/ -lR | grep ^d | wc -l)
echo $folder
now=`echo "$folder / 0.72" | bc`
dir_name=$(ls  -rt out/| tail -1)
file_name=$(ls  -rt | tail -1)
echo $now | dialog --title "LTE-SIM compiling" --gauge "Please wait , now create : \n"$file_name 9 70 0
done
