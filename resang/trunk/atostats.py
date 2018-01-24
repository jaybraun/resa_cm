#!/usr/bin/python

import sys, os, subprocess, fnmatch, platform, datetime

def atostats(root_game, atoid, atoid_update):
	import file_sort

	atoid_dir = root_game + '/' + atoid  + '/' + atoid_update + '/'
 
	# $   ato_misn_sum [game.prod.ato.'p1]naval_missions.pre atostat.tmp
	os.system('$RESA_TRUNK/ato_misn_sum.run ' + root_game + ' ' + atoid + ' ' + atoid_update + ' ' + 'nm.pre' + ' ' + 'atostat.tmp')


	file_sort.file_sort(atoid_dir + 'atostat.tmp', atoid_dir + 'atostatsort.tmp')

	#$   ato_platform_sum atostatsort.tmp atostats.tmp
	os.system('$RESA_TRUNK/ato_platform_sum.run ' + root_game + ' ' + atoid + ' ' + atoid_update + ' ' + 'atostatsort.tmp' + ' ' + 'atostats.out') 

	# $   atostats 0
	os.system('$RESA_TRUNK/ato_stats.run '  + root_game + ' ' + atoid + ' ' + atoid_update + ' ' + 'atostats.out' + ' ' + 'atoerrors.out')

	# Print summary to stdout
	print 'Resource Usage by platform for atoid ' + atoid + ' ' + atoid_update
	sf = open(atoid_dir + 'atostats.out', 'r')
	sf_text = sf.read()
	sf.close()
	print sf_text
	#with open(atoid_dir + 'atostats.out', 'r') as fin:
 	   #print fin.read() 

	print 'Resources Exceeded by platform for atoid ' + atoid + ' ' + atoid_update
	ef = open(atoid_dir + 'atoerrors.out', 'r')
	ef_text = ef.read()
	ef.close()
	print ef_text
	#with open(atoid_dir + 'atoerrors.out', 'r') as fin:
 	   #print fin.read() 

	os.remove(atoid_dir + 'atostatsort.tmp')
	os.remove(atoid_dir + 'atostat.tmp')

	return
