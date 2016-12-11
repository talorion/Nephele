TEMPLATE = subdirs

DISTFILES += Licence.txt \
               notes.txt \
		README.md

SUBDIRS +=    src
SUBDIRS +=    tests

tests.depends                += src

# build must be last:
CONFIG += ordered
