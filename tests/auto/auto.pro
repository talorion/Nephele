TEMPLATE = subdirs

#tcp_box_simulator

SUBDIRS += TcpBoxSimulator \
    TofDaqDll
SUBDIRS += libs
SUBDIRS += nephele

nephele.depends += TcpBoxSimulator

# build must be last:
#CONFIG += ordered
