#!/bin/sh
rm -f paralela.html
echo "<!DOCTYPE html><html><head><style>table, th, td {border: 1px solid black;}</style></head><body><table>" >> paralela.html
echo "<tr><td>X</td>"  >> paralela.html
for win_j in `seq 10 10 500`;do
    echo "<td>$win_j"  >> paralela.html
    echo "</td>" >> paralela.html
done
echo "</tr>" >> paralela.html

for th in `seq 1 100`;do
	echo "<tr><td>thread = $th</td>"  >> paralela.html
	for win_j in `seq 10 10 200`;do
		echo "<td>"  >> paralela.html
		./result  ../../database/personaB.avi 540 450 $win_j $win_j 200 $th >> paralela.html;
		echo "tempo-$win_i-$win_j";
		echo "</td>" >> paralela.html
		date;
	done
	echo "</tr>" >> paralela.html
done
echo "</table></body></html>" >> paralela.html
