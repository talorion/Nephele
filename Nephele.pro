TEMPLATE = subdirs

CONFIG += debug

DISTFILES += \
    doc/ToDo.txt \
    doc/git.txt \
    doc/testscript.js \
    doc/jsonResponseDiscretValue-Box.json \
    misc/build_number_generator.sh \
    misc/build_number_generator.bat \
    LICENSE.txt

SUBDIRS +=    qcustomplot \
    qtofdaq


SUBDIRS +=    nephele_core
#===Systems===
SUBDIRS +=    tcp_box_system
SUBDIRS +=    data_aquisition_dll_system
SUBDIRS +=    data_tools_dll_system
SUBDIRS +=    power_supply_dll_system
#======
SUBDIRS +=    nephele

SUBDIRS +=    tcp_box_simulator


nephele.depends                         += qcustomplot
nephele.depends                         += nephele_core
tcp_box_system.depends                  += nephele_core
data_aquisition_dll_system.depends      += nephele_core
data_tools_dll_system.depends           += nephele_core
power_supply_dll_system.depends         += nephele_core
#nephele.depends                        += tcp_box_system
#nephele.depends                        += data_aquisition_dll_system

# build must be last:
CONFIG += ordered
