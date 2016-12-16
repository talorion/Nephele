TEMPLATE = subdirs

SUBDIRS += qcustomplot \
    qtofdaqdll \
    qecmdclient

# build must be last:
CONFIG += ordered
