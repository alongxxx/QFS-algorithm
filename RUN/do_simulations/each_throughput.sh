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
TOTALNAME=""
NAME=$(echo $FILE | sed  -n -e 's/^.*Multi_\(.*\)_.*sim/\1/p')
MAXUSER=$(echo $FILE | sed  -n -e 's/^.*Multi.*_\(.*\)U.*sim/\1/p')
./compute_each_throughput.sh $FILE $MAXUSER >> $NAME.dat


echo $NAME  >> results_each_throughput_$NAME_$MAXUSER.ods
echo Users Value  >> results_each_throughput_$NAME_$MAXUSER.ods
	cat $NAME.dat >> results_each_throughput_$NAME_$MAXUSER.ods

./Graph1_user.sh User_Throughput_$NAME User-Throughput Users Throughput[bps] $NAME.dat
rm -f $NAME.dat
echo  $NAME.dat FINISHED!! 


