make
cd testFiles
./testScript.sh $@
cd ../
cp testFiles/memDump.out ./memDump.out
cd logs
less +G $(ls -ctr1 ./*.log | tail -1)
