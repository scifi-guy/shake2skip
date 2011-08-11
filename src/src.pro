TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS  += shake2skip-lib
SUBDIRS  += shake2skip-debug
SUBDIRS  += shake2skip-daemon

upstart.path = /etc/init/apps
upstart.files = data/shake2skip-daemon.conf

daemon.path = /opt/shake2skip/bin
daemon.files = data/shake2skip.sh

settings.path = /opt/shake2skip/config
settings.files = data/settings.ini.template

INSTALLS += upstart daemon settings
