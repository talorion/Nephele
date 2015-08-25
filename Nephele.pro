TEMPLATE = subdirs

SUBDIRS += \
    tcp_box_simulator \
    nephele \
    qcustomplot


# build must be last:
CONFIG += ordered
