# CVE-2020-27688

## Introduction
RVTools is an application developed by [Robware.net](https://www.robware.net/rvtools/) and is written in .NET 4.6.1. It interacts with vSphere enviroments to extract information in a CSV or XLSX format. It can be run through an GUI or using an input CSV File in which the information is stored to connect to the vSphere environment. The password in the CSV file is encrypted using a proprietary encryption application. The encrypted password can be identified by the "\_RVTools" prefix.

## Affected versions
<= 4.0.6

## Vulnerability
The encryption is configured using a static IV and KEY, using reverse engineering techiques it is possible to extract this IV and KEY. These values can be used to decrypt the password used in the configuration files.

## POC
This repository contains the .NET code to decrypt the encrypted password using the static IV and KEY.
