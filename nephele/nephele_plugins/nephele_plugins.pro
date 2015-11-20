TEMPLATE = subdirs

SUBDIRS +=    co2sensor_system
SUBDIRS +=    configuration_system
SUBDIRS +=    gui_system
SUBDIRS +=    logging_system
SUBDIRS +=    octave_system
SUBDIRS +=    plotting_system
SUBDIRS +=    scripting_system
SUBDIRS +=    tcpbox_system
SUBDIRS +=    tofdaqdll_system
SUBDIRS +=    twtooldll_system
SUBDIRS +=    twtps_system

# build must be last:
CONFIG += ordered

