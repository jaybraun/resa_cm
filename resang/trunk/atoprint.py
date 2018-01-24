#!/usr/bin/python

import sys, os, subprocess, fnmatch, platform, datetime

def atoprint(atoid_dir):

	lineprint(atoid_dir + 'atoheader.out')
	lineprint(atoid_dir + 'atoerrors.out')
	lineprint(atoid_dir + 'atomissing.out')
	lineprint(atoid_dir + 'tlam.pre.out')

	return

def lineprint(f):
	if os.path.isfile(f):
		subprocess.Popen(['lpr', f])

