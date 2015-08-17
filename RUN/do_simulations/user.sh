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
Iter=10
TOTALNAME=""
UserID=7
NAME=$(echo $FILE | sed  -n -e 's/^.*Multi_\(.*\)_.*sim/\1/p')

	
	grep "^.*DST "$UserID   $FILE  > tmp
	./compute_user.sh tmp $Iter 
	cat temporal >> User_$NAME.dat
	rm tmp

	rm temporal





echo $NAME  >> results_User_$NAME.ods
echo Users Value  >> results_User_$NAME.ods
	grep  " " User_$NAME.dat  >> results_User_$NAME.ods




./Graph1_user.sh User_$NAME TIME-Throughput TIME[sec] Throughput[Mbps] User_$NAME.dat


rm User_$NAME.dat 
echo  User_$NAME REPORT FINISHED!! 

