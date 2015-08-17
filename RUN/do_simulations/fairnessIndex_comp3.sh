# Copyright (c) 2010 
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

# params 1 MINUSERS, 2 MAXUSERS, 3 INTERVAL, 4 FILENAME, 5 FILE, 6 NUMSIM,  7 TYPE, 8 FILE_NAME_PLOT,

FILE=$5   #OUTPUT FILE NAME
NUMSIM=$6  #SIMULATIONS NUMBER
FILENAME=$4   # SIMULATION TYPE NAME
COUNT=1
CELS=$9       # NUMBER OF CELLS
TOTALNAME=""
MAXUSER=$2
NBUE=$1 #Number of UE's
# variables for values

time=${10}
total_flow=${11}

until [ $NBUE -gt $2 ]; do

	for bearer in $(seq 0 1 $(( ${NBUE}- 1 )))
	do
		echo $bearer
	done

	# bash until loop
	# GRAPHIC FOR PROPORTIONAL FAIRNESS-------------------------------------------------	
	until [ $COUNT -gt $NUMSIM ]; 
	do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_PF_"$NBUE"U"$CELS"C"".sim"
 		./compute_each_throughput.sh $TOTALNAME $MAXUSER >> tmp
		max_throughput=0
		min_throughput=1000000
		sum=0
		while read output; do		
			throughput=$(echo "$output" | grep -o "\s.*$" )
			sum=$(echo $sum+$throughput | bc -l)
			tmp=$(echo "$max_throughput<$throughput" | bc -l)
			if [ $tmp == 1 ];
  			then
				max_throughput=$(bc -l <<< $throughput )
			fi

			tmp=$(echo "$min_throughput>$throughput" | bc -l)
			if [ $tmp == 1 ];
  			then
				min_throughput=$(bc -l <<< $throughput )
			fi
		done
		fairness=$(echo ($max_throughput-$min_throughput)/$sum | bc -l)
		echo $fairness
	let COUNT=COUNT+1
	done < tmp
	COUNT=1

#START ANOTHER ALGORITHM
#
#-----> Add code here
#
#END ANOTHER ALGORITHM
 let NBUE=NBUE+$3
done

echo PF  >> results_$8_$7.ods
echo Users Value  >> results_$8_$7.ods
	grep  " " PF_Y1_$8_$7.dat  >> results_$8_$7.ods
echo MLWDF  >> results_$8_$7.ods
echo Users Value  >> results_$8_$7.ods
	grep  " " MLWDF_Y1_$8_$7.dat  >> results_$8_$7.ods
echo EXP-PF  >> results_$8_$7.ods
echo Users Value  >> results_$8_$7.ods
	grep  " " EXPPF_Y1_$8_$7.dat  >> results_$8_$7.ods
echo my_algo  >> results_$8_$7.ods
echo Users Value  >> results_$8_$7.ods
	grep  " " MY_algo_Y1_$8_$7.dat  >> results_$8_$7.ods
echo my_algo2  >> results_$8_$7.ods
echo Users Value  >> results_$8_$7.ods
	grep  " " MY_algo2_Y1_$8_$7.dat  >> results_$8_$7.ods

./Graph1.sh $7_$8 PF_Y1_$8_$7.dat MLWDF_Y1_$8_$7.dat EXPPF_Y1_$8_$7.dat $7-Fairness-Index2 Users Fairness_Index MY_algo_Y1_$8_$7.dat MY_algo2_Y1_$8_$7.dat

rm PF_Y1_$8_$7.dat 
rm MLWDF_Y1_$8_$7.dat 
rm EXPPF_Y1_$8_$7.dat 
rm MY_algo_Y1_$8_$7.dat
rm MY_algo2_Y1_$8_$7.dat
echo  FAIRNESS2 $7 REPORT FINISHED!! 


