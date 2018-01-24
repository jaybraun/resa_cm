#!/usr/bin/python

import sys, os, subprocess, fnmatch, platform, datetime, shutil, time

def atomissions(opts, ftpdir, wsn):
	import atostats
	import atoprint
	import all_files

	# Validate options

	opt_replenish = 'yes'
	opt_prescript = 'yes'
	opt_tlam = 'yes'

	for c in opts:
		if c == 'r':
			opt_replenish = 'no'
		elif c == 'p':
			opt_prescript = 'no'
		elif c == 'o':
			opt_tlam = 'no'

	# Validate directory
	if not os.path.isdir(ftpdir):
		print " ftp directory does not exist"
		sys.exit(100)

	# Validate workstation number

	if int(wsn) < 1 or int(wsn) > 64:
		print 'workstation number ' + wsn + ' out of range(1-64)'
		sys.exit(100)

	while 1 == 1:
		# Find new ato update by checking "PERMFILE" in ftp and working directories
		root_ftp = ftpdir
		#root_ftp = '/home/resa/ftp'
		root_ato = 'ato'
		lastfile_name = 'PERMFILE'
		root_ftp_len = len(root_ftp)
		root_ato_len = len(root_ato)
		new_ato_dir = ''
		empty_game = 'y'

		for path_ftp in sorted(all_files.all_files(root_ftp, lastfile_name)):
			for path_game in sorted(all_files.all_files(root_ato, lastfile_name)):
				empty_game = 'n'
				if path_game[root_ato_len:] == path_ftp[root_ftp_len:]:
					break 
				if path_game[root_ato_len:] > path_ftp[root_ftp_len:]: 
					new_ato_dir = path_ftp
					break 
			if not new_ato_dir == '':
				break
			if empty_game == 'y':
				new_ato_dir = path_ftp
				break
			if path_game[root_ato_len:] < path_ftp[root_ftp_len:]:
				new_ato_dir = path_ftp
				break 
		if not new_ato_dir == '':
	   		# Copy files from ftp to working directory structure

			path = root_ato+path_ftp[root_ftp_len:path_ftp.find(lastfile_name)]
			try: 
				os.makedirs(path)
			except OSError:
				if not os.path.isdir(path):
					raise

			srcDir = root_ftp+path_ftp[root_ftp_len:path_ftp.find(lastfile_name)]
			source = os.listdir(srcDir)
			destination = path
			for files in source:
				shutil.copyfile(srcDir+files,destination+files)

			# Extract atoid and atoid_update number from directory path
			path_list = new_ato_dir.split('/')
			atoid = path_list[len(path_list)-3]
			atoid_update = path_list[len(path_list)-2]
			atoid_dir = root_ato + '/' + atoid + '/' + atoid_update + '/'

			# Check for existence of airspace definition file
			aco_file = atoid_dir + 'ACO.' + atoid + "." + atoid_update
			aco_exists = 'y'
			if not os.path.isfile(aco_file):
				aco_exists = 'n'

			# Check for existence of ato file
			ato_exists = 'y'
			if not os.path.isfile(atoid_dir + atoid + "." + atoid_update):
				ato_exists = 'n'

			if aco_exists == 'y':
				print '~/resang/trunk/ato_airspace.run ' + root_ato + ' ' + atoid + ' ' + atoid_update
				print 'Airspace definitions by type for adoid ' + atoid + ' ' + atoid_update
   				# run rat program ato_airspace <atoid> <atoid update number>
				os.system('$RESA_TRUNK/ato_airspace.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' ' + wsn)	
			if ato_exists == 'y':
    				#run rat program ato_naval_missions <atoid> <atoid update number> <auto tlam input param>
				if os.path.isfile(atoid_dir + 'nm.pre'):
					os.remove(atoid_dir + 'nm.pre')
				print '~/resang/trunk/ato_missions.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' nm.pre atomisn.tmp ' + opt_tlam
				os.system('$RESA_TRUNK/ato_missions.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' nm.pre atomisn.tmp ' + opt_tlam)	

				#run rat program ato_onemission <current ato working directory>nm.pre
				print '~/resang/trunk/ato_onemission.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' nm.pre'
				os.system('$RESA_TRUNK/ato_onemission.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' ' + 'nm.pre')

    				#run rat program ato_oneusmtf <current ato working directory><atoid>.<atoid update number>
				print '~/resang/trunk/ato_oneusmtf.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' ' + atoid + '.' + atoid_update
				os.system('$RESA_TRUNK/ato_oneusmtf.run '  + root_ato + ' ' + atoid + ' ' + atoid_update + ' ' + atoid + '.' + atoid_update)

				# Append usmtf mission files to resa prescript files 

				for file in os.listdir(atoid_dir):
					if file.endswith(".pre"):
						usmtf_file = atoid_dir + file[:-5] + '.usmtf'
						if os.path.isfile(usmtf_file):
							p = open(atoid_dir + file, "a")
							u = open(usmtf_file, 'r')
   							p.write(u.read())
							p.close()
							u.close()
	
			# Append mission orders to airspace orders  OR run ato_prescript and prescript for each file
			airspace_file = atoid_dir + 'ato_airspace.pre'
			mission_file = atoid_dir + 'nm.pre'
			#if ato_exists == 'y' and aco_exists == 'y':
			#	file_list = [airspace_file, mission_file]
			#elif ato_exists == 'y':
			#	file_list = [mission_file]
			#elif aco_exists == 'y':
			#	file_list = [airspace_file]
			if ato_exists == 'y':				# Airspace definitions directly sent to station so only need mission file
				file_list = [mission_file]

			t = open(atoid_dir + 't.cmd', 'w')
			for myfile in file_list:
				f = open(myfile, 'r')
				t.write(f.read())
				f.close()
			t.close()

   			# Run rat program ato_prescript t.cmd nm.pre <auto replenish input flag>
			print '~/resang/trunk/ato_prescript.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' t.cmd nm.pre ' + opt_replenish
			os.system('$RESA_TRUNK/ato_prescript.run ' + root_ato + ' ' + atoid + ' ' + atoid_update + ' t.cmd nm.pre ' + opt_replenish)
			os.remove(atoid_dir + 't.cmd')

			if opt_prescript == 'yes' and ato_exists == 'y':
				# Send orders to RESA station
				#prescript [game.prod.ato.'d1.'d2]nm.pre 'wsnum YES 
				os.system('$RESA_TRUNK/tsmain.run ' + atoid_dir + 'nm.pre ' + wsn + ' NO')
				time.sleep(180)				# 3 minutes
				# $! Compare ATO missions in RESA game with ATO missions in raw ATO. Report diffs
				os.system('$RESA_TRUNK/ato_resa_misn.run '  + root_ato + ' ' + atoid + ' ' + atoid_update + ' atomisn.tmp' + ' ' + 'atomissing.out')
				# Print summary to stdout
				print 'Missing missions for atoid ' + atoid + ' ' + atoid_update
				sf = open(atoid_dir + 'atomissing.out', 'r')
				sf_text = sf.read()
				sf.close()
				print sf_text
				#with open(atoid_dir + 'atomissing.out', 'r') as fin:
 					#print fin.read()
				print 'End Missing Missions' 


			atostats.atostats(root_ato, atoid, atoid_update)

			# $! Print summary and error reports
			#atoprint.atoprint(atoid_dir)
			if opt_prescript == 'no' and ato_exists == 'y':
				print 'Verify and Edit nm.pre and then run prescript'
				print 'prescript ato/' + atoid + '/' + atoid_update + '/nm.pre ' + wsn + ' YES'
				return

		time.sleep(600)     # wait 10 minutes
		

if __name__ == '__main__':		# Invoked from command line. Not imported
	opts = ""
	args = ""
	if len(sys.argv) == 3:
		opts = ""
		arg1 = sys.argv[1]
		arg2 = sys.argv[2]
	elif len(sys.argv) == 4:
		opts = sys.argv[1]
		arg1 = sys.argv[2]
		arg2 = sys.argv[3]
 	else:
		print " Usage: atomissions [-opts] ftpdir workstation_number"
		print "                     r No replensih orders"
		print "                     p No prescript"
		print "                     t No tlam shooter assignments"
		sys.exit(100)

	atomissions(opts, arg1, arg2) 

