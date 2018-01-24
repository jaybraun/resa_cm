cd $RESA_HOME
cd db/build
blmain.run -n -i=democ.scr demo
cd ../demo
frmain.run demo.txt
cp port* $RESA_HOME/game/
cd ../sonar_env/special/
bemain.run portsea
cd $RESA_HOME/game/
wimain.run port.ini
echo "You are now in directory $PWD with an initialized Blackboard"
