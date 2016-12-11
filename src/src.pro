TEMPLATE = subdirs

SUBDIRS +=    libs
SUBDIRS +=    NepheleCore
SUBDIRS +=    NephelePlugins
SUBDIRS +=    nephele

NepheleCore.depends += libs

NephelePlugins.depends += NepheleCore

nephele.depends += NepheleCore
nephele.depends += NephelePlugins

# build must be last:
CONFIG += ordered
