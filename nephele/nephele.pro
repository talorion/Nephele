#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T18:40:47
#
#-------------------------------------------------

QT       += core gui script printsupport network scripttools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): RC_ICONS += nephele.ico
lessThan(QT_MAJOR_VERSION, 5): RC_FILE = nephele.rc

RESOURCES += \
    Resources.qrc

TARGET = nephele
TEMPLATE = app

unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
unix: QMAKE_CXXFLAGS += -isystem '$ORIGIN'/../../../libs/qcustomplot
unix: QMAKE_CXXFLAGS += -Wall
unix: QMAKE_CXXFLAGS += -Wextra
unix: QMAKE_CXXFLAGS += -ansi
unix: QMAKE_CXXFLAGS += -pedantic
unix: QMAKE_CXXFLAGS += -Wshadow
unix: QMAKE_CXXFLAGS += -Weffc++
unix: QMAKE_CXXFLAGS += -Wstrict-aliasing

CONFIG += c++11

#==========BUILD NUMBER
#unix: build_nr.commands = ../misc/build_number_generator.sh
#unix: build_nr.depends = FORCE
#unix: QMAKE_EXTRA_TARGETS += build_nr
#unix: PRE_TARGETDEPS += build_nr
unix: BUILDNO = $$system(../misc/build_number_generator.sh)

#win32: build_nr.commands = ../misc/build_number_generator.bat
#win32: build_nr.depends = FORCE
#win32: QMAKE_EXTRA_TARGETS += build_nr
#win32: PRE_TARGETDEPS += build_nr
win32: BUILDNO = $$system(../misc/build_number_generator.bat)
win32: BUILDNO = $$system(..\misc\build_number_generator.bat)
#==========


#==========Deploy
win32: {
    TARGET_CUSTOM_EXT = .exe

    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/debug/))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/release/))
    }

    DEPLOY_COMMAND = windeployqt
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} --dir $${DLLDESTDIR} --no-translations $${DEPLOY_TARGET}
}
#==========================================



SOURCES += main.cpp\
    core/event_manager.cpp \
    script_system/scripting_worker.cpp \
    script_system/script_handler/script_act_handler.cpp \
    script_system/script_handler/script_dialoges_handler.cpp \
    script_system/script_handler/script_set_handler.cpp \
    script_system/script_handler/script_util_handler.cpp \
    gui_system/analogview.cpp \
    gui_system/flowcontrollerview.cpp \
    gui_system/nephele_main_window.cpp \
    tcp_box_system/qvmbackend.cpp \
    tcp_box_system/tcpdriver.cpp \
    tcp_box_system/tcpcommand.cpp \
    tcp_box_system/tcpcommandqueue.cpp \
    tcp_box_system/flowcontrollerbackend.cpp \
    script_system/script_system.cpp \
    tcp_box_system/tcp_box_system.cpp \
    gui_system/gui_system.cpp \
    gui_system/script_editor/script_editor_window.cpp \
    gui_system/script_editor/highlighter.cpp \
    gui_system/script_editor/console.cpp \
    script_system/script_handler/script_log_handler.cpp \
    core/entity_manager.cpp \
    data_aquisition_dll_system/data_aquisition_dll_system.cpp \
    data_aquisition_dll_system/data_aquisition_dll_wrapper.cpp \
    power_supply_dll_system/power_supply_dll_system.cpp \
    script_system/script_handler/script_daq_handler.cpp \
    tcp_box_system/abstract_backend.cpp \
    gui_system/settings_dialog/settings_dialog.cpp \
    core/system_manager.cpp \
    tcp_box_system/tbs_config_widget/tbs_config_widget.cpp \
    core/abstract_configuration_widget.cpp \
    data_aquisition_dll_system/tof_daq_specific/tof_daq_dll_tools.cpp \
    data_aquisition_dll_system/dad_config_widget/dad_config_widget.cpp \
    data_aquisition_dll_system/data_aquisition_dll_worker.cpp \
    tcp_box_system/tcp_box_worker.cpp \
    core/abstract_scriptable_object.cpp

HEADERS  += version.hpp \
    nephele.rc \
    core/event_manager.hpp \
    tcp_box_system/tcpcommand.hpp \
    tcp_box_system/qvmbackend.hpp \
    tcp_box_system/tcpcommandqueue.hpp \
    tcp_box_system/tcpdriver.hpp \
    gui_system/analogview.hpp \
    gui_system/flowcontrollerview.hpp \
    gui_system/nephele_main_window.hpp \
    script_system/script_handler/script_act_handler.hpp \
    script_system/script_handler/script_dialoges_handler.hpp \
    script_system/script_handler/script_set_handler.hpp \
    script_system/script_handler/script_util_handler.hpp \
    script_system/scripting_worker.hpp \
    tcp_box_system/flowcontrollerbackend.h \
    script_system/script_system.hpp \
    tcp_box_system/tcp_box_system.hpp \
    core/abstract_system.hpp \
    gui_system/gui_system.hpp \
    gui_system/script_editor/script_editor_window.hpp \
    gui_system/script_editor/highlighter.h \
    gui_system/script_editor/console.h \
    script_system/script_handler/script_log_handler.hpp \
    core/entity_manager.hpp \
    tcp_box_system/tcpdriverdatatypes.hpp \
    data_aquisition_dll_system/data_aquisition_dll_wrapper.hpp \
    data_aquisition_dll_system/data_aquisition_dll_system.hpp \
    power_supply_dll_system/power_supply_dll_system.hpp \
    script_system/script_handler/script_daq_handler.hpp \
    tcp_box_system/abstract_backend.hpp \
    gui_system/settings_dialog/settings_dialog.hpp \
    core/system_manager.hpp \
    tcp_box_system/tbs_config_widget/tbs_config_widget.hpp \
    core/abstract_configuration_widget.hpp \
    data_aquisition_dll_system/tof_daq_specific/tof_daq_dll_tools.hpp \
    data_aquisition_dll_system/dad_config_widget/dad_config_widget.hpp \
    build_number.h \
    data_aquisition_dll_system/data_aquisition_dll_worker.hpp \
    tcp_box_system/tcp_box_worker.hpp \
    core/abstract_scriptable_object.hpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/release/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/debug/ -lqcustomplot
else:unix: LIBS += -L$$OUT_PWD/../qcustomplot/ -lqcustomplot

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/release/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/debug/libqcustomplot.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/release/qcustomplot.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/debug/qcustomplot.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/libqcustomplot.a

unix: INCLUDEPATH += $$PWD/../../../libs/TofDaq_1.97_API/include
unix: DEPENDPATH += $$PWD/../../../libs/TofDaq_1.97_API/include

DISTFILES += \
    nephele.ico \
    build_number.txt



