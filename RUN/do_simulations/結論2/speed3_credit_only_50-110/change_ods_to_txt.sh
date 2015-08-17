echo $pwd
for f in *ods
do
	mv $f $(echo $f | sed 's/ods/txt/g')
done

for f in *txt
do
	echo $f
	sed -i 's/[0-9]* \(.*\)/\1/g' $f
done
