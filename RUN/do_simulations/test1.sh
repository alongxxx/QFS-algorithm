mkdir out2/
cd ./out/
dir=.
for f in "$dir"/*; do
	 folder=$(echo $f | sed  -n -e 's/^.\/\(.*\)/\1/p') 
	 echo $folder
	 mkdir ../out2/$folder/
	 cp ./$folder/*jpg ../out2/$folder/
done
