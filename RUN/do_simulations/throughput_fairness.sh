#The Edge# Copyright (c) 2010 
# 
# This file is part of LTE-Sim
# LTE-Sim is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation;
# 
# LTE-Sim is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with LTE-Sim; if not, see <http://www.gnu.org/licenses/>.
#
# Author: Mauricio Iturralde <mauricio.iturralde@irit.fr, mauro@miturralde.com>

# params 1 MINUSERS, 2 MAXUSERS, 3 INTERVAL, 4 FILENAME, 5 FILE, 6 NUMSIM, 7 TYPE, 8 FILE_NAME_PLOT,

iter=$1
if [ $# -eq 0  ]; then
	iter=10
fi

Min_user=$(cat results_ThroughputMB_Total.ods | grep -A 2 "^PF$" | grep -A 1 "Users Value" | grep -v "Users Value" | grep -o "^.*\s")
Max_user=$(cat results_ThroughputMB_Total.ods | grep -B 1 "^MLWDF$" | grep -v "^MLWDF$" | grep -o "^.*\s")

let grep_line=(Max_user-Min_user)/iter+2
let grep_line2=(Max_user-Min_user)/iter+1

array=()
array1=()
cat results_ThroughputMB_Total.ods | grep "^[0-9]* [0-9].*" > tmp
max=0
i=0
while read line
do
	array1[$i]=$(echo $line | grep -o "\s.*$")
        tmp=$(echo "${array1[$i]}>$max" | bc -l)
	if [ $tmp == 1 ]
  	then
		max=$(bc -l <<< ${array1[$i]})
	fi
let i=i+1
done < tmp


#------------------PF---------------------------
array=()
array1=()
cat results_ThroughputMB_Total.ods | grep -A $grep_line  "^PF$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
i=0
while read line
do
	var1=$(echo $line | grep -o "^.*\s")
	array[$i]=$var1
	array1[$i]=$(echo $line | grep -o "\s.*$")
let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    array1[$i]=$(echo ${array1[$i]}/$max | bc -l)
done
i=0
cat results_Fairness-Index_Total.ods | grep -A $grep_line  "^PF$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
while read line
do
	var1=$(echo $line | grep -o "\s.*$")
	array1[$i]=$(echo ${array1[$i]}+$var1 | bc -l)
	let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    echo ${array[$i]} ${array1[$i]} >> PF.dat
done

#------------------MLWDF---------------------------
array=()
array1=()
cat results_ThroughputMB_Total.ods | grep -A $grep_line  "^MLWDF$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
i=0
while read line
do
	var1=$(echo $line | grep -o "^.*\s")
	array[$i]=$var1
	array1[$i]=$(echo $line | grep -o "\s.*$")
let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    array1[$i]=$(echo ${array1[$i]}/$max | bc -l)
done
i=0
cat results_Fairness-Index_Total.ods | grep -A $grep_line  "^MLWDF$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
while read line
do
	var1=$(echo $line | grep -o "\s.*$")
	array1[$i]=$(echo ${array1[$i]}+$var1 | bc -l)
	let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    echo ${array[$i]} ${array1[$i]} >> MLWDF.dat
done

#------------------EXP-PF---------------------------
array=()
array1=()
cat results_ThroughputMB_Total.ods | grep -A $grep_line  "^EXP-PF$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
i=0
while read line
do
	var1=$(echo $line | grep -o "^.*\s")
	array[$i]=$var1
	array1[$i]=$(echo $line | grep -o "\s.*$")
let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    array1[$i]=$(echo ${array1[$i]}/$max | bc -l)
done
i=0
cat results_Fairness-Index_Total.ods | grep -A $grep_line  "^EXP-PF$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
while read line
do
	var1=$(echo $line | grep -o "\s.*$")
	array1[$i]=$(echo ${array1[$i]}+$var1 | bc -l)
	let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    echo ${array[$i]} ${array1[$i]} >> EXP-PF.dat
done

#------------------my_algo---------------------------
array=()
array1=()
cat results_ThroughputMB_Total.ods | grep -A $grep_line  "^my_algo$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
i=0
while read line
do
	var1=$(echo $line | grep -o "^.*\s")
	array[$i]=$var1
	array1[$i]=$(echo $line | grep -o "\s.*$")
let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    array1[$i]=$(echo ${array1[$i]}/$max | bc -l)
done
i=0
cat results_Fairness-Index_Total.ods | grep -A $grep_line  "^my_algo$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
while read line
do
	var1=$(echo $line | grep -o "\s.*$")
	array1[$i]=$(echo ${array1[$i]}+$var1 | bc -l)
	let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    echo ${array[$i]} ${array1[$i]} >> my_algo.dat
done

#------------------my_algo2---------------------------
array=()
array1=()
cat results_ThroughputMB_Total.ods | grep -A $grep_line  "^my_algo2$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
i=0
while read line
do
	var1=$(echo $line | grep -o "^.*\s")
	array[$i]=$var1
	array1[$i]=$(echo $line | grep -o "\s.*$")
let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    array1[$i]=$(echo ${array1[$i]}/$max | bc -l)
done
i=0
cat results_Fairness-Index_Total.ods | grep -A $grep_line  "^my_algo2$" | grep -A $grep_line2 "Users Value" |grep -v "Users Value" >tmp
while read line
do
	var1=$(echo $line | grep -o "\s.*$")
	array1[$i]=$(echo ${array1[$i]}+$var1 | bc -l)
	let i=i+1
done < tmp
rm tmp
for (( i=0; i<$(( ${#array1[@]} )); i++ ))
do
    echo ${array[$i]} ${array1[$i]} >> my_algo2.dat
done

./Graph1.sh Throughput-Fairness PF.dat MLWDF.dat EXP-PF.dat Throughput-Fairness Users Throughput-Fairness my_algo.dat my_algo2.dat

rm PF.dat 
rm MLWDF.dat 
rm EXP-PF.dat 
rm my_algo.dat
rm my_algo2.dat
echo  thoughput_fairness REPORT FINISHED!! 


