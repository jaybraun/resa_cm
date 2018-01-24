#!/bin/bash
#set -o nounset # Complain if reference unset variable
#set -o errexit # Exit if command has error
#set -o xtrace
#set -o verbose
# Copysave - copy scenario files necessary for restore to backup server

#infinite loop
#No loop since time-out when no password entered
#while [ 1 ]
#do
	file_list=""
	for i in $(find $RESA_HOME/game -newer $RESA_HOME/game/touched.file); do
		# process file extensions: sve, eng, ord, log, tim
		if [[ $i = *.sve || $i = *h.eng || $i = *h.ord || $i = *.log || $i = *h.tim ]]
		then	
			# make list of updated files with space a separator
			file_list="$file_list $i"
			echo $i
		fi
	done

	# Bell for password alert
	echo -en "\07"

	# Copy changed files to backup server.  User will be prompted for password
	scp $file_list resa@alpha3:$RESA_HOME/game/backup
	#scp $file_list /home/resa/resa_root/resa_build/game/backup
	echo "copy complete at"
	date

	# Update time touched
	touch $RESA_HOME/game/touched.file

	#wait for 1 hour
	#sleep 1h
#done

