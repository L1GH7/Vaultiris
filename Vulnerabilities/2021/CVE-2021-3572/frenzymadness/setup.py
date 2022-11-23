from cve_2021_3572 import version
from setuptools import setup

setup(
    name='cve_2021_3572',
    version=version,
    description='Project to help test CVE-2021-3572 in pip',
    url='http://github.com/lbalhar/CVE-2021-3572',
    author='Lumír Balhar',
    author_email='frenzy.madness@gmail.com',
    license='MIT',
    py_modules=['cve_2021_3572'],
    zip_safe=False
)
