#!/usr/bin/env python

import sys, random, re
if len(sys.argv) > 1: MAXDELAY = int(sys.argv[1])
else: MAXDELAY = 50

# interpose.c template strings
header = ""
libraries = "#define _GNU_SOURCE\n" \
	"#define HAVE_GETUID\n" \
	"#define HAVE_GETEUID\n" \
	"#define HAVE_GETGID\n" \
	"#define HAVE_PTHREAD_MUTEXATTR_INIT\n" \
	"#include <dlfcn.h>\n" \
	"#include <unistd.h>\n" \
	"#include <sys/types.h>\n" \
	"#include <stdbool.h>\n" \
	"#include \"/usr/include/time.h\"\n" \
	"#include <pthread.h>\n" \
	"#include \"libvirt/libvirt.h\"\n" \
	"#include \"hash.h\"\n" \
	"#include \"virnetserverclient.h\"\n" \
	"#include \"virnetserverprogram.h\"\n" \
	"#include \"domain_event.h\"\n\n"
func_head = "{ret}{space}{func_name}({func_args})\n"
# add "{\n"
func_body1 = "\tstatic {ret} (*real_{func_name})({func_args_no_id}) = NULL;\n\tint i;\n"
func_timespec1 = "\tstruct timespec req = {(time_t) 0, (long) "
func_timespec2 = "{delay}"
func_timespec3 ="};\n\tnanosleep(&req, NULL);\n"
func_timespec ="\tfor(i=0; i<{delay}; i++)\n\t\tasm(\"nop;\");\n"
func_body2 = "\tif(!real_{func_name})\n"
func_body3 = "\t\treal_{func_name} = dlsym(RTLD_NEXT, \"{func_name}\");\n"
func_ret = "\treturn real_{func_name}({func_args_id});\n" \
# add"}"

interpose_file = header + libraries

# splits types and ids into seperate lists, returns a tuple of 2 lists
# list 1 is of types, list 2 is of ids
# 'restrict' keyword is filtered out, and "..." is filtered out
def split_func_args(func_args):
	ret_args = ([],[])
	args = func_args.split(',')
	for arg in args:
		arg = arg.strip()
		if (arg.rfind("*") >= 0): # If parameter is of type pointer
			star = arg.rfind("*")+1
			arg_type = arg[0:star]
			id = arg[star:len(arg)].replace("restrict", " ").strip()
			ret_args[0].append(arg_type)
			ret_args[1].append(id)
		elif(arg.rfind(" ") >= 0):
			space = arg.rfind(" ")
			arg_type = arg[0:space]
			id = arg[space:len(arg)].replace("restrict", " ").strip()
			ret_args[0].append(arg_type)
			ret_args[1].append(id)
		elif(arg == "void"):
			ret_args[0].append(arg)
			ret_args[1].append("")
		elif(arg.find("...")): # for variable arguments
			print(arg)
	return ret_args

# takes in a list of function argument types
def get_func_args_no_id(func_args_type):
	return ", ".join(func_args_type)

# takes in a list of function arguments
def get_func_args(func_args):
	new_func_list = []
	for i in range(0,len(func_args[0])):
		new_func_list.append(func_args[0][i] + " " + func_args[1][i])
	return ", ".join(new_func_list)

# takes in a list of function argument id
def get_func_args_id(func_args_id):
	return ", ".join(func_args_id)


pthread_re = re.compile('(^\[pid \d+\] pthread_.*)|(^\[pid \d+\] virMutex.*)|(^\[pid \d+\] virThread.*)')
function_re = re.compile('^\[pid \d+\] (.*)\(.*$')
LTRACE_IGNORE = [
	pthread_re,
	re.compile('^\[pid \d+\] --- .*'),
	re.compile('^\[pid \d+\] \+\+\+ .*'),
	re.compile('^\[pid \d+\] __.*'),
	re.compile('^\[pid \d+\] <\.\.\. .*')
]

functions = set()
ltrace = open('ltracelibvirtd.out', 'r')
pthread_found = False
for line in ltrace:
	if pthread_found:
		for expression in LTRACE_IGNORE:
			if expression.match(line): break
		else:
			functions.add(function_re.match(line).group(1))
			pthread_found = False
	elif pthread_re.match(line):
		pthread_found = True
ltrace.close()

FUNCTION_EXPRESSIONS = list()
for function in functions:
	print function
	FUNCTION_EXPRESSIONS.append(re.compile('^.* \*?' + function + '\(.*\);'))

with open("prototypes.txt") as file:
	for line in file:
		function_found = False
		for expression in FUNCTION_EXPRESSIONS:
			if expression.match(line):
				function_found = True
		if not function_found: continue
		line = line.strip()
		if line is "":
			continue

		# Libraries have # in front
		elif line[0] is "#":
			continue

		# If no # in front and ends with ;, then assume it is a function
		elif line[len(line)-1] is ";":
			if line.count(';') > 1:
				print("Error(3): " + line)
				continue

			if line.count('.') == 3:
				# Future work: manage variable length arguments
				continue

			# func_decl[0] is everything before '(''
			func_decl = line.split('(', 1)
			pre_paren = func_decl[0]
			func_args_str = func_decl[1][0:len(func_decl[1])-2] # strip away ");"
			if (pre_paren.rfind("*") >=0): # If return type is of pointer
				star = pre_paren.rfind("*")+1
				ret_type = pre_paren[0:star]
				func_name = pre_paren[star:len(pre_paren)].strip()
				func_args = split_func_args(func_args_str)

				# Create function using template strings
				function = func_head.format(ret=ret_type,space="",
					func_name=func_name, func_args=get_func_args(func_args)) + "{\n"
				function += func_body1.format(ret=ret_type,
					func_name=func_name, func_args_no_id=get_func_args_no_id(func_args[0]))
				function += func_timespec.format(delay=random.randint(0, MAXDELAY))
				function += func_body2.format(func_name=func_name)
				function += func_body3.format(func_name=func_name)
				function += func_ret.format(func_name=func_name,
					func_args_id=get_func_args_id(func_args[1])) + "}\n"
				interpose_file += function + "\n"

			elif (pre_paren.rfind(" ") >= 0):
				space = pre_paren.rfind(" ")
				ret_type = pre_paren[0:space]
				func_name = pre_paren[space:len(pre_paren)].strip()
				func_args = split_func_args(func_args_str)

				# Create function using template strings
				function = func_head.format(ret=ret_type,space=" ",
					func_name=func_name, func_args=get_func_args(func_args)) + "{\n"
				function = function + func_body1.format(ret=ret_type,
					func_name=func_name, func_args_no_id=get_func_args_no_id(func_args[0]))
				function += func_timespec.format(delay=random.randint(0, MAXDELAY))
				function += func_body2.format(func_name=func_name)
				function += func_body3.format(func_name=func_name)
				function += func_ret.format(func_name=func_name,
					func_args_id=get_func_args_id(func_args[1])) + "}\n"
				interpose_file += function + "\n"

			else:
				print("Error(2): " + pre_paren)
				continue
		else:
			print("Error(1): " + line)
			continue

with open("interpose.c", "w") as outfile:
	outfile.write(interpose_file)
