#!/usr/bin/env python
"""
Runs test with MAX_DELAY ranging from min to max.
"""
import argparse, subprocess, os, sys

parser = argparse.ArgumentParser()
parser.add_argument('-S', '--start', help='Minimum value for MAX_DELAY', default=0, type=int)
parser.add_argument('-E', '--end', help='Maximum value for MAX_DELAY', default=1000000, type=int)
parser.add_argument('-R', '--repeat', help='Number of inner loop iterations', default=1, type=int)
args = parser.parse_args()

start = args.start
end = args.end
repeat = args.repeat
FNULL = open(os.devnull, 'w')

try:
	os.mkdir('test-results')
except OSError:
	print "Error: test-results directory already exists"
	sys.exit()

# x is MAX_DELAY
for x in xrange(start, end, 1000):
	total_caught = 0
	try:
		dir = subprocess.Popen(['mkdir', './test-results/{max_delay}'.format(max_delay=x)])
		dir.communicate()
	except:
		print("Error(mkdir-x): Cannot make test results directory {m}".format(m=x))
	# Inner loop repeat 'repeat' times
	for i in xrange(repeat):
		is_good = True
		print('Randomizing for MAX_DELAY {max_delay}, iteration {num}'.format(max_delay=x, num=i))
		try:
			p = subprocess.Popen(['make', 'random', 'MAX_DELAY='+str(x)], stdout=FNULL, stderr=FNULL)
			p.communicate()
		except:
			is_good = False
			print('Error(make random): MAX_DELAY {max_delay}, iteration {num}'.format(max_delay=x, num=i))
		try:
			r = subprocess.check_output(['./repeatbug.py'])
			linesarray = r.split('\n')
			runs = linesarray[5].split()[0]
			crashtime = linesarray[6].split()[0]
		except:
			is_good = False
			print('Error(repeatbug.py): MAX_DELAY {max_delay}, iteration {num}'.format(max_delay=x, num=i))
		try:
			m = subprocess.check_output(['./microbench.sh'])
			time = m.split()[0]
		except:
			is_good = False
			print('Error(microbench.sh): MAX_DELAY {max_delay}, iteration {num}'.format(max_delay=x, num=i))
		if is_good:
			try:
				dir2 = subprocess.Popen(['mkdir', './test-results/{max_delay}/{iteration}'.format(max_delay=x, iteration=i)])
				dir2.communicate()
			except:
				print("Error(mkdir-i): Cannot make test results directory {i}".format(i=i))
			try:
				with open("./test-results/{m}/{i}/{m}.{i}.csv".format(m=x, i=i), "w") as file:
					file.write("{rboutput}\n{r},{c},{t}\n".format(rboutput=r,r=runs,c=crashtime,t=time))
			except:
				print("Error(file): Cannot write to file {m}.{i}.csv".format(m=x, i=i))
			try:
				with open("./test-results/all-results.csv", "a") as file:
					for j in range(5):
						file.write("{m},{i},{this_run},{r},{c},{t}\n".format(m=x, i=i, this_run=linesarray[j], r=runs, c=crashtime, t=time))
			except:
				print("Error(file): Cannot write to file all-results.csv")
			try:
				interpose = subprocess.Popen(['cp','interpose.c', './test-results/{max_delay}/{iteration}'.format(max_delay=x, iteration=i)])
				interpose.communicate()
				print("Runs per failure:{r}; Time per failure:{c}; Microbenchmark: {t} microseconds".format(r=runs, c=crashtime, t=time))
			except:
				print("Error(interpose): Cannot copy interpose.c to {m}/{i}".format(m=x, i=i))
