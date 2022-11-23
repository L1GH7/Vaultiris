#!/bin/bash
MYDIR=$(dirname "$(readlink -f "$0")")
sudo docker run --rm -ti -v "$MYDIR/exploit:/home/editor/exploit" justinsteven/sudo_digest_race bash
