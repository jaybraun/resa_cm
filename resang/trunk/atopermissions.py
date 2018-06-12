#!/usr/bin/python

import sys, os, time
import all_files

def atopermissions(ftpdir):

	# Find new ato update by checking "LASTFILE" in ftp directories
	# Change LASTFILE to PERMFILE to trigger atomissions.py to cycle
	root_ftp = ftpdir
	lastfile_name = 'LASTFILE'
	permfile_name = 'PERMFILE'
	root_ftp_len = len(root_ftp)

	while 1 == 1:

		for path_ftp in sorted(all_files.all_files(root_ftp, lastfile_name)):
			srcDir = root_ftp+path_ftp[root_ftp_len:path_ftp.find(lastfile_name)]
			if not os.path.isfile(srcDir + permfile_name):
				os.system('chmod 775'+' '+srcDir+'/*')
				os.system('mv'+' '+path_ftp+' '+srcDir+'/'+permfile_name)

		time.sleep(60)     # wait 1 minute
		

if __name__ == '__main__':		# Invoked from command line. Not imported
	opts = ""
	args = ""
	if len(sys.argv) == 2:
		arg1 = sys.argv[1]
 	else:
		print " Usage: atopermissions ftpdir"
		sys.exit(100)

	atopermissions(arg1) 

