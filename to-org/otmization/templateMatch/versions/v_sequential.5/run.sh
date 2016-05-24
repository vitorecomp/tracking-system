#!/bin/sh
rm -f seqeuncial.html
echo "<!DOCTYPE html><html><head><style>table, th, td {border: 1px solid black;}</style></head><body><table>" >> seqeuncial.html
echo "<tr><td>X</td>"  >> seqeuncial.html
for win_j in `seq 10 10 500`;do
    echo "<td>$win_j"  >> seqeuncial.html
    echo "</td>" >> seqeuncial.html
done
echo "</tr>" >> seqeuncial.html

for th in 1;do
	echo "<tr><td>thread = $th</td>"  >> seqeuncial.html
	for win_j in `seq 10 10 500`;do
		echo "<td>"  >> seqeuncial.html
		./result  ../../database/personaB.avi 540 450 $win_j $win_j 200 >> seqeuncial.html;
		echo "tempo-$win_i-$win_j";
		echo "</td>" >> seqeuncial.html
		date;
	done
	echo "</tr>" >> seqeuncial.html
done
echo "</table></body></html>" >> seqeuncial.html
