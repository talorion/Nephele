TEMPLATE = subdirs

CONFIG += debug

DISTFILES += \
    doc/ToDo.txt \
    doc/git.txt \
    doc/testscript.js \
    doc/jsonResponseDiscretValue-Box.json \
    misc/build_number_generator.sh \
    misc/build_number_generator.bat

SUBDIRS +=    qcustomplot
SUBDIRS +=    tcp_box_simulator
SUBDIRS +=    nephele


nephele.depends           += qcustomplot

# build must be last:
CONFIG += ordered
