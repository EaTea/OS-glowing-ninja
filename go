make
cd testFiles
./testScript.sh
cd ../
cd logs
less +G $(ls -ctr1 ./*.log | tail -1)
