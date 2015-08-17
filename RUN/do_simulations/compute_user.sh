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
now_time=0
file=$1
iter=$2
sum=0
count1=$iter

	
while read output; do
	first=$(echo "$output" | awk '{print $1}' )
	if [ "$first" == "RX" ]; then
		throughput=$(echo "$output" | awk '{print $8+5}' )	
	else 
		now_time=$(echo "$output" | awk '{print $14 }')
	fi
	#echo $now_time	
		
	
	a=$(echo "$now_time>=$count1" | bc)
	if [ $a == 1 ]; 
	then
		let avg=sum/iter
		let avg=avg*8/1000
		echo $count1" "$avg
		echo $count1" "$avg >> temporal
		let count1=count1+iter
		sum=0
	else
		let sum=sum+throughput	
	fi


done < $1

