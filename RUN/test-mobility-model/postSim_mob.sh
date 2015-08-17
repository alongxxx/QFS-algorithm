file_in=output/test_mob
file_out_x=output/test_mob_POS_X
file_out_y=output/test_mob_POS_Y

echo "GREP on ${file_in}"

grep "UserPosition X" ${file_in} | awk 'BEGIN{ORS="";}{ for (i=0; i<70; i++) print $(i+5)," "; print "\n"; }'  > ${file_out_x}
grep "UserPosition Y" ${file_in} | awk 'BEGIN{ORS="";}{ for (i=0; i<70; i++) print $(i+5)," "; print "\n"; }'  > ${file_out_y}
              




 
 

  

