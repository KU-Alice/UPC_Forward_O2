#echo "removing old file "
#rm output.txt
#echo "finding AO2D.root and saving location to output.txt"
#find ~/Run3/ \( -name "*AO2D.root" ! -name *.txt  \) >>output.txt
echo "running o2 analysis"
alienv setenv  O2/latest -c o2-analysis-timestamp | o2-analysis-upc-forward | o2-analysis-event-selection --aod-file @output_alienlocation.txt   -b
