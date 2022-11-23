#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sysconfig

#install_scheme = 'nt' if (os.name == 'nt') else 'posix_prefix'
scripts = sysconfig.get_path('scripts')
# print(scripts)
base = sysconfig.get_config_var('base')
# print(base)
end = scripts.removeprefix(base)

userbase = sysconfig.get_config_var('userbase')
# print(userbase)

print("Check " + userbase + end + "/ is included in your PATH environment variable for system wide execution.")

# answer = input("Add the python scripts directory to the PATH environment variable for system wide execution?\n"
#                "e.g. export PATH=$PATH:" + userbase + end + "/\ny/n:")
# if answer == "y" or answer == "yes" or answer == "Y" or answer == "Yes":
#     # Append python scripts dir to PATH in bash
#     file_object = open("~/.bashrc", "a")
#     file_object.write("export PATH=$PATH:" + userbase + end)
#     file_object.close()
#     print(userbase + end + " added to PATH.")
#     os.system("echo $PATH")
#     exit(2)
# elif answer == "n" or answer == "no" or answer == "N" or answer == "No":
#     print("If you need system wide execution, add the python scripts directory to the PATH enviroment variable. " + userbase + end + "/")
#     exit(3)
# else:
#     print("yes or no required, y/n, yes/no")
#     exit(4)



    


