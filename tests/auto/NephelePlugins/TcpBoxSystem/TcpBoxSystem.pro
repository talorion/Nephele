TEMPLATE = subdirs

SUBDIRS += TcpBoxSimulator \
    TcpBoxSystemHw \
    TcpBoxSystem
SUBDIRS += EcmdConnection

EcmdConnection.depends += TcpBoxSimulator

CONFIG += ordered
