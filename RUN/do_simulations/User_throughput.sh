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

NBUE=$1 #Number of UE's
# variables for values

time=${10}

mkdir User_throughput

until [ $NBUE -gt $2 ]; do

	# bash until loop
	# GRAPHIC FOR PROPORTIONAL FAIRNESS-------------------------------------------------	
	until [ $COUNT -gt $NUMSIM ]; do
	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_PF_"$NBUE"U"$CELS"C"".sim"
	./compute_user_throughput.sh $TOTALNAME
	 let COUNT=COUNT+1
	done	
	COUNT=1
	mv User_*ods User_throughput/

    #GRAPHIC FOR M-LWDF --------------------------------------------------------------------
	until [ $COUNT -gt $NUMSIM ]; do
	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MLWDF_"$NBUE"U"$CELS"C"".sim"
	./compute_user_throughput.sh $TOTALNAME
	 let COUNT=COUNT+1
	done	
	COUNT=1
	mv User_*ods User_throughput/

    #GRAPHIC FOR EXP/PF----------------------------------------------------------------------
	until [ $COUNT -gt $NUMSIM ]; do
	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_EXPPF_"$NBUE"U"$CELS"C"".sim"
	./compute_user_throughput.sh $TOTALNAME
	 let COUNT=COUNT+1
	done	
	COUNT=1
	mv User_*ods User_throughput/

    #GRAPHIC FOR my_algo-----------------------------------------------------------------------
	until [ $COUNT -gt $NUMSIM ]; do
	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MY_algo_"$NBUE"U"$CELS"C"".sim"
	./compute_user_throughput.sh $TOTALNAME
	 let COUNT=COUNT+1
	done	
	COUNT=1
	mv User_*ods User_throughput/
	
    #GRAPHIC FOR my_algo2---------------------------------------------------------------------
	until [ $COUNT -gt $NUMSIM ]; do
	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MY_algo2_"$NBUE"U"$CELS"C"".sim"
	./compute_user_throughput.sh $TOTALNAME
	 let COUNT=COUNT+1
	done	
	COUNT=1
	mv User_*ods User_throughput/

 let NBUE=NBUE+$3
done

