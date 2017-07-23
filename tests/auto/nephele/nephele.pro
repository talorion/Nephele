TEMPLATE = subdirs

SUBDIRS += ManagerLocator
SUBDIRS += tcpDriver
SUBDIRS += tcp_box_view
SUBDIRS += EntityManager
SUBDIRS += SystemManager
SUBDIRS += tcp_box_system
SUBDIRS += tbs_config_widget
SUBDIRS += TcpBoxEntity
SUBDIRS += JsonRpcDriver
SUBDIRS += JsonRpc
SUBDIRS += JsonRpcDriver_HW
SUBDIRS += DaqDllEntity
SUBDIRS += DataAquisitionConfigurationWidget
SUBDIRS += DataAquisitionSystem
SUBDIRS += DataAquisitionWorker

# build must be last:
CONFIG += ordered
