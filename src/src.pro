TEMPLATE = subdirs

SUBDIRS +=    libs
SUBDIRS +=    nepheleLib
SUBDIRS +=    nephele

nepheleLib.depends += libs

nephele.depends += nepheleLib
nephele.depends += libs

# build must be last:
#CONFIG += ordered
