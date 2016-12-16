TEMPLATE = subdirs

SUBDIRS += TcpBoxSimulator
SUBDIRS += TcpBoxSystemHw
SUBDIRS += TcpBoxSystem
SUBDIRS += EcmdConnection

EcmdConnection.depends += TcpBoxSimulator
TcpBoxSystemHw.depends += TcpBoxSimulator

CONFIG += ordered
