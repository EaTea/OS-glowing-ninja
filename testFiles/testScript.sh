#!/bin/bash
#REMEMBER TO MAKE THIS FILE EXECUTABLE
echo "Attempting to run ../build/scheduler with each file with a .jobs extension"
for i in *.jobs 
	do
	echo "file = $i"
	echo "RR: Round Robin Scheduling Algorithm"
	for k in $@
		do
		echo "Time Quantum: $k"
		../build/scheduler RR $k $i
	done
	echo "FCFS: First Come First Serve Scheduling Algorithm"
	../build/scheduler FCFS $i
done
mv program.log ../logs/$(date '+%F-%T')-program.log
echo "The .log file has been moved to ../logs."
