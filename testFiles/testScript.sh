#!/bin/bash
#REMEMBER TO MAKE THIS FILE EXECUTABLE
for i in *.file 
	do
	for k in {2,5,8,10,12,15,20,30,32,50,65}
		do
		../build/scheduler RR $k $i
	done
	../build/scheduler FCFS $i
done
mv program.log ../logs/$(date '+%F-%T')-program.log
