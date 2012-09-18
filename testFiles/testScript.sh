#!/bin/bash
#REMEMBER TO MAKE THIS FILE EXECUTABLE
echo "Attempting to run ../build/os-project with each file with a .jobs extension"
for i in *.jobs 
	do
	echo "file = $i"
	echo "RR: Round Robin Scheduling Algorithm"
#TODO: Make this prettier. Possibilities include putting into a file? Making this list user specified?
#http://stackoverflow.com/questions/3939196/run-command-line-arguments-in-python-script 
#the above link recommends using $@
	for k in {2,5,8,10,12,15,20,30,32,50,65}
		do
		echo "Time Quantum: $k"
		../build/os-project RR $k $i
	done
	echo "FCFS: First Come First Serve Scheduling Algorithm"
	../build/os-project FCFS $i
done
mv program.log ../logs/$(date '+%F-%T')-program.log
echo "The .log file has been moved to ../logs."
