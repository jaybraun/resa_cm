def file_sort(input_file, output_file):
	import os
	f = open(input_file,'r')
	lines = f.readlines()
	f.close()
	lines.sort()
	f = open(output_file,'w')
	for line in lines:
		f.write(line)
	f.close()

#file_sort('sortinput', 'sortoutput')
