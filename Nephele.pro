TEMPLATE = subdirs

DISTFILES += Licence.txt \
		README.md

SUBDIRS +=    nephele
SUBDIRS +=    nephele_test
SUBDIRS +=    libs


# build must be last:
CONFIG += ordered
