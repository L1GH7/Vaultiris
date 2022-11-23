#!/bin/bash

set -ex

idfile=$(mktemp /tmp/foo.XXXXXXXXX)

docker build --iidfile ${idfile} .

buildid=`cat ${idfile}`

docker rmi ${buildid}
rm -f "${idfile}"
