FILE=$1
MAXUSER=$2
for (( c=1; c<=$MAXUSER; c++ ))
do	
	throughput=$(grep "^RX.*DST "$c" D"   $FILE |awk '{print $8+5}'  | awk 'BEGIN {s=0;} {s+=$1;} END {printf "%4.5f\n", s}') 
	echo $c $throughput 
done
