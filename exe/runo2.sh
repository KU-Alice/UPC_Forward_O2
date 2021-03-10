alienv setenv  O2/latest -c bash -c
echo "removing old file "
rm output.txt
"finding AO2D.root and saving location to output.txt"
find ~/ \( -name "*AO2D.root" ! -name *.txt  \) >>output.txt
echo "running o2 analysis"
o2-analysis-timestamp | o2-analysis-upc-forward --aod-file @output.txt [--isPbPb]  --aod-memory-rate-limit 10000000000 --shm-segment-size 16000000000 -b
#exit
