MASTERFILES ?= /var/cfengine/masterfiles
PROJECT=$(shell basename $(CURDIR) )
INSTALL_TARGET=${MASTERFILES}/services/${PROJECT}
FILES=$(shell find .)
install: ${FILES}
	mkdir -p ${INSTALL_TARGET}/
	rsync -avz --exclude \.#.* ${CURDIR}/* ${INSTALL_TARGET}/
	find ${INSTALL_TARGET}/ -type f | xargs chmod 600
	find ${INSTALL_TARGET}/ -type d | xargs chmod 700
