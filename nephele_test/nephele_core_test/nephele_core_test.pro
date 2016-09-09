#-------------------------------------------------
#
# Project created by QtCreator 2015-11-20T18:26:02
#
#-------------------------------------------------

QT          += testlib
QT          -= gui
QT          += network

TARGET    = tst_entity_manager
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

TEMPLATE = app

include(../../common_cxxflags.pri)


SOURCES += tst_entity_manager.cpp \
    main.cpp \
    tst_system_manager.cpp \
    system_dummy.cpp \
    tst_system.cpp \
    tst_logging_system.cpp \
    tst_tcpbox_system.cpp \
    tst_tcpbox_hw.cpp \
    tcp_box_simulator/abstract_simulated_box.cpp \
    tcp_box_simulator/simulated_ao.cpp \
    tcp_box_simulator/simulated_ao_box.cpp \
    tcp_box_simulator/simulated_av.cpp \
    tcp_box_simulator/simulated_av_box.cpp \
    tcp_box_simulator/simulated_fc.cpp \
    tcp_box_simulator/simulated_fc_box.cpp \
    tcp_box_simulator/tcp_box_connection.cpp \
    tcp_box_simulator/tcp_box_server.cpp \
    tcp_box_simulator/tcp_box_simulator.cpp \
    tcp_box_simulator/tcp_box_simulator_thread.cpp \
    tst_ecmd_connection.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    tst_entity_manager.hpp \
    tst_system_manager.hpp \
    system_dummy.hpp \
    tst_system.hpp \
    tst_logging_system.hpp \
    tst_tcpbox_system.hpp \
    tst_tcpbox_hw.hpp \
    tcp_box_simulator/abstract_simulated_box.hpp \
    tcp_box_simulator/simulated_ao.hpp \
    tcp_box_simulator/simulated_ao_box.hpp \
    tcp_box_simulator/simulated_av.hpp \
    tcp_box_simulator/simulated_av_box.hpp \
    tcp_box_simulator/simulated_fc.h \
    tcp_box_simulator/simulated_fc_box.h \
    tcp_box_simulator/tcp_box_connection.h \
    tcp_box_simulator/tcp_box_server.h \
    tcp_box_simulator/tcp_box_simulator.hpp \
    tcp_box_simulator/tcp_box_simulator_thread.hpp \
    tst_ecmd_connection.hpp

# ===========================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_core/release/ -lnephele_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_core/debug/ -lnephele_core
else:unix: LIBS += -L$$OUT_PWD/../../nephele/nephele_core/ -lnephele_core

INCLUDEPATH += $$PWD/../../nephele/nephele_core
DEPENDPATH += $$PWD/../../nephele/nephele_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/release/libnephele_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/debug/libnephele_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/release/nephele_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/debug/nephele_core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/libnephele_core.a
# ===========================

# ===========================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/logging_system/release/ -llogging_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/logging_system/debug/ -llogging_system
else:unix: LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/logging_system/ -llogging_system

INCLUDEPATH += $$PWD/../../nephele/nephele_plugins/logging_system
DEPENDPATH += $$PWD/../../nephele/nephele_plugins/logging_system

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/release/liblogging_system.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/debug/liblogging_system.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/release/logging_system.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/debug/logging_system.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/liblogging_system.a
# ===========================

# ===========================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/release/ -ltcpbox_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/debug/ -ltcpbox_system
else:unix: LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/ -ltcpbox_system

INCLUDEPATH += $$PWD/../../nephele/nephele_plugins/tcpbox_system
DEPENDPATH += $$PWD/../../nephele/nephele_plugins/tcpbox_system

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/release/libtcpbox_system.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/debug/libtcpbox_system.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/release/tcpbox_system.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/debug/tcpbox_system.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/tcpbox_system/libtcpbox_system.a
# ===========================


