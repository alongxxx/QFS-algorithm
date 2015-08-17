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

# params 1 MINUSERS, 2 MAXUSERS, 3 INTERVAL, 4 FILENAME, 5 FILE, 6 NUMSIM, 7 TYPE, 8 FILE_NAME_PLOT,

FILE=$1   #OUTPUT FILE NAME
NAME=$(echo $FILE | sed  -n -e 's/^.*Multi_\(.*\)_.*sim/\1/p')
MAXUSER=5
NBUE=$(echo $FILE | sed  -n -e 's/^.*Multi.*_\(.*\)U.*sim/\1/p')
CELS=$(echo $FILE | sed  -n -e 's/^.*Multi.*U\(.*\)C.*sim/\1/p')

Video_Edge=()
Video_Center=()
VOIP_Edge=()
VOIP_Center=()
CBR_Edge=()
CBR_Center=()

while read line
do
	#-------------------------video-------------------------------------
	if [[ "$line" =~ "RX VIDEO".*"C 0" ]]; then
		user=$(echo $line | grep -oh "DST [0-9]* " | grep -oh "[0-9]*")
		bit=$(echo $line | awk '{print $8+5}')
		Video_Edge[$user]=$(( ${Video_Edge[$user]}+$bit )) 
	fi
	if [[ "$line" =~ "RX VIDEO".*"C 1" ]]; then
		user=$(echo $line | grep -oh "DST [0-9]* " | grep -oh "[0-9]*")
		bit=$(echo $line | awk '{print $8+5}')
		Video_Center[$user]=$(( ${Video_Center[$user]}+$bit ))	
	fi

	#--------------------------voip ------------------------------------
	if [[ "$line" =~ "RX VOIP".*"C 0" ]]; then
		user=$(echo $line | grep -oh "DST [0-9]* " | grep -oh "[0-9]*")
		bit=$(echo $line | awk '{print $8+5}')
		VOIP_Edge[$user]=$(( ${VOIP_Edge[$user]}+$bit ))
	fi
	if [[ "$line" =~ "RX VOIP".*"C 1" ]]; then
		user=$(echo $line | grep -oh "DST [0-9]* " | grep -oh "[0-9]*")
		bit=$(echo $line | awk '{print $8+5}')
		VOIP_Center[$user]=$(( ${VOIP_Center[$user]}+$bit ))
	fi

	#----------------------------CBR----------------------------------------
	if [[ "$line" =~ "RX CBR".*"C 0" ]]; then
		user=$(echo $line | grep -oh "DST [0-9]* " | grep -oh "[0-9]*")
		bit=$(echo $line | awk '{print $8+5}')
		CBR_Edge[$user]=$(( ${CBR_Edge[$user]}+$bit ))
	fi
	if [[ "$line" =~ "RX CBR".*"C 1" ]]; then
		user=$(echo $line | grep -oh "DST [0-9]* " | grep -oh "[0-9]*")
		bit=$(echo $line | awk '{print $8+5}')
		CBR_Center[$user]=$(( ${CBR_Center[$user]}+$bit ))
	fi	
done < $FILE

for ((i=CELS;i<(NBUE*CELS+CELS);i++)); do 
	
	#-------------------VIDEO--------------------------------
	if [ -n "${Video_Edge[$i]}" ]; then
    		echo $i ${Video_Edge[$i]} >> "User_"$NAME"_"$NBUE"_Video_Edge.ods"
	else
    		echo $i 0 >> "User_"$NAME"_"$NBUE"_Video_Edge.ods"
	fi

	if [ -n "${Video_Center[$i]}" ]; then
    		echo $i ${Video_Center[$i]} >> "User_"$NAME"_"$NBUE"_Video_Center.ods"
	else
    		echo $i 0 >> "User_"$NAME"_"$NBUE"_Video_Center.ods"
	fi

	#-----------------VOIP----------------------------------
	if [ -n "${VOIP_Edge[$i]}" ]; then
    		echo $i ${VOIP_Edge[$i]} >> "User_"$NAME"_"$NBUE"_VOIP_Edge.ods"
	else
    		echo $i 0 >> "User_"$NAME"_"$NBUE"_VOIP_Edge.ods"
	fi

	if [ -n "${VOIP_Center[$i]}" ]; then
    		echo $i ${VOIP_Center[$i]} >> "User_"$NAME"_"$NBUE"_VOIP_Center.ods"
	else
    		echo $i 0 >> "User_"$NAME"_"$NBUE"_VOIP_Center.ods"
	fi
	
	#----------------CBR------------------------------------
	if [ -n "${CBR_Edge[$i]}" ]; then
    		echo $i ${CBR_Edge[$i]} >> "User_"$NAME"_"$NBUE"_CBR_Edge.ods"
	else
    		echo $i 0 >> "User_"$NAME"_"$NBUE"_CBR_Edge.ods"
	fi

	if [ -n "${CBR_Center[$i]}" ]; then
    		echo $i ${CBR_Center[$i]} >> "User_"$NAME"_"$NBUE"_CBR_Center.ods"
	else
    		echo $i 0 >> "User_"$NAME"_"$NBUE"_CBR_Center.ods"
	fi
done
