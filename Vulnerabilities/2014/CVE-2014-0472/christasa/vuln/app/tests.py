import os

from django.test import TestCase

# Create your tests here.


def evil_function():
    print("exploit successful")
    os.system("echo > /tmp/`whoami` ")
    return os.getuid()


evil = evil_function()
