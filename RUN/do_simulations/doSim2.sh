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


# Single Cell With Interference

FILE="Sim"   #OUTPUT FILE NAME
NUMSIM=1  #Number of simulations
FILENAME="Multi"   # SIMULATION TYPE NAME
COUNT=1
CELS=1       # NUMBER OF CELLS
TOTALNAME=""
MINUSERS=50 # Start users
INTERVAL=10 # Interval between users
MAXUSERS=120 #max of users 
# params of LTE-SIM MULTICEL

RADIUS=3 # Radius in Km
NBUE=0 #Number of UE's
NBVOIP=1  # Number of Voip Flows
NBVIDEO=1 #Number of Video
NBBE=0 # Number of Best Effort Flows
NBCBR=1 #Number of CBR flows
#Scheduler Type PF=1, MLWDF=2 EXP= 3  
FRAME_STRUCT=1  # FDD or TDD
SPEED=3 #User speed 
MAXDELAY=0.1
VIDEOBITRATE=128
DATA_RATE=2

#-------------------------------------------------------------------------------------------------------------------------
content=0    # 1 means execute all the scheduler  ,  0 means ignore all the schedulers  remember!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#-------------------------------------------------------------------------------------------------------------------------

time=$(sed  -n -e 's/^.*double duration =\( *\)\(.*\);/\2/p' ../../src/scenarios/multi-cell.h)
bandwidth=$(sed  -n -e 's/^.*double bandwidth =\( *\)\(.*\);/\2/p' ../../src/scenarios/multi-cell.h)
bandwidth=`echo "$bandwidth * 1000000" | bc`

NBUE=$MINUSERS
until [ $NBUE -gt $MAXUSERS ]; do
if [ "$content" -gt 0 ]
then
	
	# bash until loop
	until [ $COUNT -gt $NUMSIM ]; do
	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_PF_"$NBUE"U"$CELS"C"".sim"
	../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 1 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE > $TOTALNAME
        	echo FILE $TOTALNAME CREATED!
 	let COUNT=COUNT+1
	done
	COUNT=1
	

	until [ $COUNT -gt $NUMSIM ]; do
       	TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MLWDF_"$NBUE"U"$CELS"C"".sim"
	../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 2 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE > $TOTALNAME
       	echo FILE $TOTALNAME CREATED!
	 let COUNT=COUNT+1
	done
	COUNT=1
	
	until [ $COUNT -gt $NUMSIM ]; do
  	      TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_EXPPF_"$NBUE"U"$CELS"C"".sim"
	../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 3 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE > $TOTALNAME
   	    echo FILE $TOTALNAME CREATED!
 	let COUNT=COUNT+1
	done
	COUNT=1
	
	#My algo
	until [ $COUNT -gt $NUMSIM ]; do
  	      TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MY_algo_"$NBUE"U"$CELS"C"".sim"
	../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 7 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE > $TOTALNAME
	    echo ../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 7 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE 
   	    echo FILE $TOTALNAME CREATED!
 	let COUNT=COUNT+1
	done
	COUNT=1
	
	#My algo2
	until [ $COUNT -gt $NUMSIM ]; do
  	      TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MY_algo2_"$NBUE"U"$CELS"C"".sim"
	../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 8 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE > $TOTALNAME
	    echo ../../LTE-Sim MultiCell $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 8 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE $COUNT $DATA_RATE 
   	    echo FILE $TOTALNAME CREATED!
 	let COUNT=COUNT+1
	done
	COUNT=1
	
fi

let NBUE=NBUE+INTERVAL
done
echo SIMULATION FINISHED!
echo CREATING RESULTS REPORT!

# params 1 MINUSERS, 2 MAXUSERS, 3 INTERVAL, 4 FILENAME, 5 FILE, 6 NUMSIM, 7 TypeFlow, Graphic_name

# result shells 
./cell_compute/total/cell_center/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Center_Packet-Loss-Ratio $CELS $time
./cell_compute/total/cell_center/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Center_ThroughputMB $CELS $time
./cell_compute/total/cell_center/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Center_Throughput $CELS $time
./cell_compute/total/cell_center/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Center_Delay $CELS $time
./cell_compute/total/cell_center/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  Total Cell_Center_Fairness-Index $CELS $time
#./cell_compute/total/cell_center/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  Total Cell_Center_Fairness-Index2 $CELS $time

./cell_compute/total/cell_edge/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Edge_Packet-Loss-Ratio $CELS $time
./cell_compute/total/cell_edge/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Edge_ThroughputMB $CELS $time
./cell_compute/total/cell_edge/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Edge_Throughput $CELS $time
./cell_compute/total/cell_edge/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Cell_Edge_Delay $CELS $time
./cell_compute/total/cell_edge/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  Total Cell_Edge_Fairness-Index $CELS $time
#./cell_compute/total/cell_edge/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  Total Cell_Edge_Fairness-Index2 $CELS $time

./cell_compute/total/total/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Packet-Loss-Ratio $CELS $time
./cell_compute/total/total/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total ThroughputMB $CELS $time
./cell_compute/total/total/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Throughput $CELS $time
./cell_compute/total/total/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Total Delay $CELS $time
./cell_compute/total/total/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  Total Fairness-Index $CELS $time
#./cell_compute/total/total/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  Total Fairness-Index2 $CELS $time

./spectral_efficiency_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Spectral-Efficiency Spec-Eff $CELS $time $bandwidth
./throughput_fairness.sh $INTERVAL
./throughput_fairness2.sh $INTERVAL


if [ $NBVOIP -gt 0 ]
then
	./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Packet-Loss-Ratio $CELS $time
	./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP ThroughputMB $CELS $time	
	./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Throughput $CELS $time
	./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Delay $CELS $time
	./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Fairness-Index $CELS $time
	#./fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Fairness-Index2 $CELS $time
	
	./cell_compute/cell_center/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Center_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_center/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Center_ThroughputMB $CELS $time
	./cell_compute/cell_center/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Center_Throughput $CELS $time
	./cell_compute/cell_center/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Center_Delay $CELS $time
	./cell_compute/cell_center/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Cell_Center_Fairness-Index $CELS $time
	#./cell_compute/cell_center/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Cell_Center_Fairness-Index2 $CELS $time
	
	./cell_compute/cell_edge/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Edge_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_edge/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Edge_ThroughputMB $CELS $time
	./cell_compute/cell_edge/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Edge_Throughput $CELS $time
	./cell_compute/cell_edge/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Cell_Edge_Delay $CELS $time
	./cell_compute/cell_edge/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Cell_Edge_Fairness-Index $CELS $time
	#./cell_compute/cell_edge/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Cell_Edge_Fairness-Index2 $CELS $time
	
fi

if [ $NBVIDEO -gt 0 ]
then
	./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Packet-Loss-Ratio $CELS $time
	./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO ThroughputMB $CELS $time
	./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Throughput $CELS $time
	./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Delay $CELS $time
	./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Fairness-Index $CELS $time
	#./fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Fairness-Index2 $CELS $time
	
	./cell_compute/cell_center/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Center_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_center/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Center_ThroughputMB $CELS $time
	./cell_compute/cell_center/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Center_Throughput $CELS $time
	./cell_compute/cell_center/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Center_Delay $CELS $time
	./cell_compute/cell_center/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Cell_Center_Fairness-Index $CELS $time
	#./cell_compute/cell_center/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Cell_Center_Fairness-Index2 $CELS $time
	
	./cell_compute/cell_edge/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Edge_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_edge/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Edge_ThroughputMB $CELS $time
	./cell_compute/cell_edge/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Cell_Edge_Throughput $CELS $time
	./cell_compute/cell_edge/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Cell_Edge_Delay $CELS $time
	./cell_compute/cell_edge/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Cell_Edge_Fairness-Index $CELS $time
	#./cell_compute/cell_edge/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Cell_Edge_Fairness-Index2 $CELS $time
	
fi

if [ $NBBE -gt 0 ]
then
	./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Packet-Loss-Ratio $CELS $time
	./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF ThroughputMB $CELS $time
	./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Throughput $CELS $time
	./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Delay $CELS $time
	./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Fairness-Index $CELS $time
	#./fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Fairness-Index2 $CELS $time
	
	./cell_compute/cell_center/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Center_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_center/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Center_ThroughputMB $CELS $time
	./cell_compute/cell_center/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Center_Throughput $CELS $time
	./cell_compute/cell_center/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Center_Delay $CELS $time
	./cell_compute/cell_center/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Cell_Center_Fairness-Index $CELS $time
	#./cell_compute/cell_center/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Cell_Center_Fairness-Index2 $CELS $time
	
	./cell_compute/cell_edge/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Edge_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_edge/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Edge_ThroughputMB $CELS $time
	./cell_compute/cell_edge/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Edge_Throughput $CELS $time
	./cell_compute/cell_edge/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Cell_Edge_Delay $CELS $time
	./cell_compute/cell_edge/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Cell_Edge_Fairness-Index $CELS $time
	#./cell_compute/cell_edge/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Cell_Edge_Fairness-Index2 $CELS $time
	
fi

if [ $NBCBR -gt 0 ]
then
	./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Packet-Loss-Ratio $CELS $time
	./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR ThroughputMB $CELS $time
	./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Throughput $CELS $time
	./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Delay $CELS $time
	./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  CBR Fairness-Index $CELS $time
	#./fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  CBR Fairness-Index2 $CELS $time
	
	./cell_compute/cell_center/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Center_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_center/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Center_ThroughputMB $CELS $time
	./cell_compute/cell_center/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Center_Throughput $CELS $time
	./cell_compute/cell_center/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Center_Delay $CELS $time
	./cell_compute/cell_center/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  CBR Cell_Center_Fairness-Index $CELS $time
	#./cell_compute/cell_center/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  CBR Cell_Center_Fairness-Index2 $CELS $time
	
	./cell_compute/cell_edge/packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Edge_Packet-Loss-Ratio $CELS $time
	./cell_compute/cell_edge/thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Edge_ThroughputMB $CELS $time
	./cell_compute/cell_edge/thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Edge_Throughput $CELS $time
	./cell_compute/cell_edge/delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Cell_Edge_Delay $CELS $time
	./cell_compute/cell_edge/fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  CBR Cell_Edge_Fairness-Index $CELS $time
	#./cell_compute/cell_edge/fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  CBR Cell_Edge_Fairness-Index2 $CELS $time
fi

./move

#./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Packet-Loss-Ratio $CELS $time
#./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Packet-Loss-Ratio $CELS $time
#./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Packet-Loss-Ratio $CELS $time
#./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Throughput $CELS $time
#./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Throughput $CELS $time
#./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Throughput $CELS $time 
#./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Delay $CELS $time
#./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Delay $CELS $time
#./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Delay $CELS $time
#./spectral_efficiency_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Spectral-Efficiency Spec-Eff $CELS $time
#./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Fairness-Index $CELS $time
#./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VOIP Fairness-Index $CELS $time
#./fairnessIndex_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  INF_BUF Fairness-Index $CELS $time
#./move
#./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Throughput $CELS $time
#./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Throughput $CELS $time
#./thoughput_compMB.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Throughput $CELS $time
#./fairnessIndex_comp2.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM  VIDEO Fairness-Index2 $CELS $time 
