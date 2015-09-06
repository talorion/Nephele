TEMPLATE = subdirs

CONFIG += debug

DISTFILES += \
    doc/ToDo.txt \
    doc/git.txt \
    doc/testscript.js \
    misc/build_number_generator.sh \
    misc/build_number_generator.bat

SUBDIRS +=    tcp_box_simulator
SUBDIRS +=    nephele
SUBDIRS +=    qcustomplot

nephele.depends           += qcustomplot

# build must be last:
CONFIG += ordered
