#!/bin/sh

echo "Starting shake2skip daemon"

#Check whether the log folder exists
[[ ! -d /home/user/.shake2skip ]] && mkdir /home/user/.shake2skip
[[ ! -e /home/user/.shake2skip/shake2skip-data.log ]] && touch /home/user/.shake2skip/shake2skip-data.log

exec /opt/shake2skip/bin/shake2skip-daemon
