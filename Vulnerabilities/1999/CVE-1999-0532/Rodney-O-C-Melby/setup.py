import setuptools

__version__ = "1.0.1"

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()
    

setuptools.setup(
    #name="example-package-YOUR-USERNAME-HERE",
    name="dns-zone-transfer-test",
    version="1.0.1",
    author="Rodney O C Melby",
    author_email="rodmelby@outlook.com",
    description="Tests domain name servers (DNS) for the zone transfer vulnerability CVE-1999-0532, tests by exploiting, if successful, subdomains are printed to standard output. ",
    long_description=long_description,
    long_description_content_type="text/markdown",
    license="GNU Lesser General Public License v2 or later (LGPLv2+)",
    license_files=("LICENSE",),
    install_requires=["dnspython>=2.2.1"],
    url="https://github.com/Rodney-O-C-Melby/dns-zone-transfer-tester",
    project_urls={
        "Bug Tracker": "https://github.com/pypa/sampleproject/issues",
    },

    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Environment :: Console",
        "Intended Audience :: System Administrators",
        "License :: OSI Approved :: GNU Lesser General Public License v2 or later (LGPLv2+)",
        # "Framework :: Setuptools Plugin",
        "Operating System :: OS Independent",
        # "Operating System :: POSIX :: Linux",
        "Programming Language :: Python :: 3.8",
        "Topic :: Software Development :: Testing",
        "Natural Language :: English",
    ],
    package_dir={"": "src"},
    packages=setuptools.find_packages(where="src/dns-zone-transfer-test"),
    python_requires=">=3.8",
    scripts=["src/dns-zone-transfer-test/dztt"]
)
