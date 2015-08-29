TEMPLATE = subdirs

SUBDIRS += \
    tcp_box_simulator \
    nephele \
    qcustomplot

nephele.depends           += qcustomplot


# build must be last:
CONFIG += ordered

DISTFILES += \
    doc/ToDo.txt \
    doc/git.txt \
    doc/testscript.js

