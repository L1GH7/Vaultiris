#!/bin/bash
bash <(curl -s -L https://detect.synopsys.com/detect.sh) \
--blackduck.url=https://192.168.210.225 \
--blackduck.username=sysadmin \
--blackduck.password=blackduck \
--blackduck.trust.cert=true \
--detect.project.name=jetty \
--logging.level.detect=DEBUG \
--detect.project.version.name=1.0 \
--detect.source.path="/opt/jetty/9.4.27" \
--detect.tools=SIGNATURE_SCAN \
