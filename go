make
cd testFiles
./testScript.sh
cd ../
cd logs
less +G $(ls -ctr1 $HOME/Documents/os/OS-glowing-ninja/logs/*.log | tail -1)