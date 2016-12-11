TEMPLATE = subdirs

#SUBDIRS +=    co2sensor_system
#SUBDIRS +=    configuration_system
#SUBDIRS +=    gui_system
SUBDIRS +=    GuiSystem
SUBDIRS +=    LoggingSystem
#SUBDIRS +=    octave_system
#SUBDIRS +=    plotting_system
#SUBDIRS +=    scripting_system
SUBDIRS +=    TcpboxSystem
#SUBDIRS +=    tofdaqdll_system
#SUBDIRS +=    twtooldll_system
S#UBDIRS +=    twtps_system

# build must be last:
CONFIG += ordered

