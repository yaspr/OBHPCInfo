#!/bin/bash 

# AUTOMATION SCRIPT (Running, Formatting, Plotting, Information gathering, ...) #

#Check script command line parameters
if [ $# -eq 0 ]
then
    
    echo "Usage: "$0" [binary file to run]"
    exit 1

fi

#Cache memory levels - This is a bash array
labels=('L1' 'L2' 'L3' 'RAM')

#Number of samples to collect per cache level
nsamples=(31 31 11 3)

#Number of runs to perform for each cache level  
nruns=(333 111 33 11)

#Array dimensions that fit into each cache level
# Rows   Cols   Elem size   Num arrays
#   30 *   30 *        8B * 3          =    21600B ~    21KB        <   32KB L1 cache
#   80 *   80 *        8B * 3          =   153600B ~   153KB        <  256KB L2 cache
#  300 *  300 *        8B * 3          =  2160000B ~  2160KB ~  2MB < L3 cache
# 1000 * 1000 *        8B * 3          = 24000000B ~ 24000KB ~ 24MB < RAM 

#This is a bash list
nbelems="30 80 300 1000"

#./bin N N_SAMPLES N_RUNS
bin=$1

#Check input file status exist and is executable
if [ -x $bin ]
then
    
    #My version of doing nothing
    echo -n ""
    
else
    
    echo "Error: cannot find binary file: "$bin

    exit 1
fi

#Initialize to integer 0
i=$(( 0 ))

datad="data_"$bin

#Remove old data directory
rm -Rf $datad

#Create new data directory
mkdir -p $datad

#Perform runs, collect & store program output
for n in $nbelems
do
    #Calculate size in bytes
    s=$(( 8 * $n * $n ))

    #Print the run parameters
    echo " ==> N: "$n" S: "$(( $s / 1024 ))"KiB Samples: "${nsamples[$i]}" Runs: "${nruns[$i]}

    #Execute the binary
    "./"$bin $n ${nsamples[$i]} ${nruns[$i]} > "$datad/data_"$bin"_"${labels[$i]}".raw"

    #Increment array index
    i=$(( $i + 1 ))

    #New line
    echo ""
    
done

#Format raw performance data for GNUPlot
for f in $datad/*.raw
do
    #Go through each line of the file
    for i in $( seq 1 5 )
    do
	#Read one line at a time & only select 1st, 2nd, and 11th columns separated by ';'.
	#In other words: code version title, memory size in bytes, bandwidth in GiB/s
	line=$( cut -d';' -f1,2,11 $f | head -n $i | tail -1 )
	
	#Extract code version title
	version=$( echo $line | cut -d';' -f1 | sed 's/ /_/g') 
	
	#Extract data (memory size and bandwidth)
	data=$( echo $line | cut -d';' -f2,3 )
		
	#Aggregate the data for each version into a single file
	echo $data >> "$datad/$version.dat"
    done
done

#Get CPU model
cpu_model=$( grep -i "model name" /proc/cpuinfo | uniq | cut -d':' -f2 )
cpu_cache=$( grep -i "cache size" /proc/cpuinfo | uniq | cut -d':' -f2 )

#
echo "set terminal png font arial 14 size 1920,1080" > $datad/plot.gp
echo "set title \"Performance comparison of different matrix multiplication implementations\"" >> $datad/plot.gp
echo "set label \"$cpu_model with $cpu_cache L3 cache, 256KiB L2 cache, 32KiB L1 cache\" at screen 0.35, 0.9 font \"arial,10\"" >> $datad/plot.gp
echo "set xlabel \"Array size in Bytes\""        >> $datad/plot.gp
echo "set ylabel \"Bandwidth in GiB/s\""         >> $datad/plot.gp

#New line
echo ""                                        >> $datad/plot.gp

#
echo -n "plot "                                >> $datad/plot.gp

tab=""

#Generate GNUPlot script
for f in $datad/*.dat
do
    version=$( basename $f .dat | sed 's/_/ /g' | sed 's/+//g')
    
    echo -e "$tab\"$f\" w lp lw 2 t \"$version\" smooth unique, \\"  >> $datad/plot.gp
    
    tab="\t"
    
done

pngf=$datad"/"$bin"_perf_plot.png"

gnuplot -e "load '$datad/plot.gp'" > $pngf

echo "Info: A plot was generated in '$pngf'"

#
rm -Rf config

#
mkdir -p config

cat /proc/cpuinfo >> config/cpuinfo.txt

#Get cache configuration details (cache line size, ) index
cp -R /sys/devices/system/cpu/cpu0/cache/index0 config/L1
cp -R /sys/devices/system/cpu/cpu0/cache/index2 config/L2
cp -R /sys/devices/system/cpu/cpu0/cache/index3 config/L3

echo "Info: The machine cache configuration can be found in 'config' directory"
