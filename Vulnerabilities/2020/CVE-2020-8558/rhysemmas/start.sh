#!/bin/sh

# Install cURL and check network conntectivity
echo "=== Testing network"
apk add curl
curl http://google.com
status=$?
if [ $status -ne 0 ]; then
  echo "Error exiting with: $status"
  exit $status
fi

# Start the exploit
echo "=== Starting exploit"
python3 /martian_packets/main.py
status=$?
if [ $status -ne 0 ]; then
  echo "Error exiting with: $status"
  exit $status
fi
exit 0
