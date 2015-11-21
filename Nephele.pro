TEMPLATE = subdirs

DISTFILES += Licence.txt \
		README.md

SUBDIRS +=    libs
SUBDIRS +=    nephele
SUBDIRS +=    nephele_test


nephele.depends                         += libs
nephele_test.depends                    += nephele


# build must be last:
CONFIG += ordered
