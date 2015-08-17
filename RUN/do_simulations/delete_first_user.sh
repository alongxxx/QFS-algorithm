for f in *txt
do
	echo $f
	sed -i 's/[0-9]* \(.*\)/\1/g' $f
done
