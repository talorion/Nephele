TEMPLATE = subdirs

CONFIG += debug

DISTFILES += LICENSE.txt \
    doc/ToDo.txt \
    doc/git.txt \
    doc/testscript.js \
    doc/jsonResponseDiscretValue-Box.json


SUBDIRS +=    qcustomplot
SUBDIRS +=    tcp_box_simulator
SUBDIRS +=    qtofdaq

SUBDIRS +=    Nephele
SUBDIRS +=    Nephele_test

Nephele.depends                         += qcustomplot
Nephele.depends                         += qtofdaq

Nephele_test.depends                    += Nephele


# build must be last:
CONFIG += ordered
