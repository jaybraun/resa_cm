cd $RESA_HOME
cd db/build
blmain.run -n -i=democ_casp.scr demo
cd ../demo
frmain.run demo_casp.txt
cp port* $RESA_HOME/game/
cd ../sonar_env/special/
bemain.run portsea
cd $RESA_HOME/game/
cp DTED.AA DTED_korea.AA
cp DTED_casp.AA DTED.AA
wimain.run port_casp.ini
echo "You are now in directory $PWD with an initialized Caspian Blackboard"
