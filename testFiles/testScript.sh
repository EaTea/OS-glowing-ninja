#!/bin/bash
#REMEMBER TO MAKE THIS FILE EXECUTABLE
echo "Attempting to run ../build/os-project with each file with a .jobs extension"
for i in *.jobs 
	do
	echo "file = $i"
	if [ "$#" -gt "0" ];
	then
		echo "RR: Round Robin Scheduling Algorithm"
	fi
		for k in $@
		do
		echo "Time Quantum: $k"
		../build/os-project RR $k $i memDump.out
		../build/os-project -m 10 20 30 RR $k $i memDump.out
	done
	echo "FCFS: First Come First Serve Scheduling Algorithm"
	../build/os-project -m 5 10 20 FCFS $i memDump.out
done
mv program.log ../logs/$(date '+%F-%T')-program.log
echo "The .log file has been moved to ../logs."
