#!/bin/bash
echo "removing the files.txt"
rm files.txt
echo "removing alienfile.txt"
rm alienfile.txt
alienv setenv O2/latest -c alien_find /alice/data/2015/LHC15o/000246392/pass5_lowIR/PWGZZ/Run3_Conversion/138_20210129-0800_child_1/AOD/ AO2D.root >>files.txt
input="files.txt"
dir=0;
echo "reading files.txt and writing it on alienfile.txt"
while IFS= read -r line
do
  #linstring = $line
  printf "alien://%s\n" $line  >>alienfile.txt
  #echo "$dir"
  #rm -r $dir
  #mkdir  $dir
  #cd $dir
  #alien_cp alien:$line file:./
  #cd ../

  #dir=$(($dir+1))

done < "$input"
rm files.txt
echo "running o2 analysis"
alienv setenv O2/latest -c bash -c
#o2-analysis-timestamp --aod-file @alienfile.txt [--isPbPb] | o2-analysis-event-selection | o2-analysis-trackextension | o2-analysis-trackselection | o2-analysis-upc-forward --aod-memory-rate-limit 10000000000 --shm-segment-size 16000000000 -b
o2-analysis-timestamp | o2-analysis-upc-forward --aod-file @alienfile.txt [--isPbPb] --aod-memory-rate-limit 10000000000 --shm-segment-size 16000000000 -b

echo "dir will be built in hour_month_day_year format"
# date and time can be found form https://www.cyberciti.biz/faq/unix-linux-bash-get-time/
now=`date +"%H_%b_%d_%y"`
mkdir $now
echo "moving files to the dir: $now "
mv alienfile.txt ./$now/
mv AnalysisResults.root ./$now/
echo "analysis completed and results can be found in: $now"
