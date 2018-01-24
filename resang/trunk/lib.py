#!/usr/bin/python

import sys, os, subprocess, fnmatch, platform, datetime

def lib(d_opts, filepattern):

	start_time = datetime.datetime.now()

	# Process arguments
	if filepattern == "":
		print "Usage: lib -<opt> <file.lib>"
		print "   Opt: c create new library"
		print "   Opt: r insert file members (with replacement)"
		sys.exit(100)
	# add wildcard to end of string if not present
	myfilepattern = filepattern
	if not myfilepattern.endswith("*"):
		myfilepattern = myfilepattern + "*"


	c_opts = ""
	for c in d_opts:
		if c == 'c':
			c_opts = c_opts + 'c'
		elif c == 'r':
			c_opts = c_opts + 'r'


	# find all .lib files in current working directory for g77
	nlibs = 0
	for file in sorted(os.listdir(os.getcwd())):
		if fnmatch.fnmatch(file, myfilepattern) and file.endswith(".lib"):
			print "Create Library ", file
			os.system('rm -f' + ' lib' + file[:-4] + '.a')
			f = open(file)				# ar @file does not accept wildard *, so read file
			arline = f.readline()
			arline = arline[:-1]
			f.close()
			os.system('ar ' + c_opts + ' ' + arline + ' >&' +file[:-4] + '.err')
			os.system('cat ' + file[:-4] + '.err')
			nlibs = nlibs + 1

	print "Execution Statistics on node ", platform.node()
	print "   LIB files processed:   ", nlibs
	diff_time = datetime.datetime.now() - start_time
	print "   Elapsed Time:        ", diff_time

	return nlibs;
if __name__ == '__main__':
	opts = ""
	args = ""
	if len(sys.argv) >= 2:
		opts = sys.argv[1]
	if len(sys.argv) >= 3:
		args = sys.argv[2]

	lib(opts, args)
