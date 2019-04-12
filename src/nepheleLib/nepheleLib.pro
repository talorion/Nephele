#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T18:40:47
#
#-------------------------------------------------

include(../common_cxxflags.pri)
include(../build_number.pri)
include(../common_flags.pri)
#QT      += core
#QT      += scripttools

BUILD_NUMBER = 650

TARGET = nepheleLib
TEMPLATE = lib
#VERSION = 0.0.$$BUILD_NUMBER
VERSION = 2.0.0

#DEFINES += BUILD_DATE='"$(shell date)"'
#DEFINES += BUILD='"$$BUILD_NUMBER"'

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d) # if compiling in debug mode, append a "d" to the library name
  QMAKE_TARGET_PRODUCT = "nepheleLib (debug mode)"
  QMAKE_TARGET_DESCRIPTION = "Control Software for PTR3 (debug mode)"
} else {
  QMAKE_TARGET_PRODUCT = "nepheleLib"
  QMAKE_TARGET_DESCRIPTION = "Control Software for PTR3"
}
QMAKE_TARGET_COMPANY = "github.com/talorion/Nephele"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) by Gregor Mayramhof"

#win32: {
#    TARGET_CUSTOM_EXT = .dll

#    CONFIG( debug, debug|release ) {
#        # debug
#        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
#        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/debug/))
#    } else {
#        # release
#        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
#        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/release/))
#    }

#    DEPLOY_COMMAND = windeployqt
#    QMAKE_POST_LINK = $${DEPLOY_COMMAND} --dir $${DLLDESTDIR} --no-translations $${DEPLOY_TARGET}
#}

SOURCES += \
    core/abstract_configuration_widget.cpp \
    core/abstract_scriptable_object.cpp \
    core/abstract_system.cpp \
    core/AbstractAction.cpp \
    core/AbstractSystemView.cpp \
    core/EntityManager.cpp \
    core/EventManager.cpp \
    core/ManagerLocator.cpp \
    core/SystemEntity.cpp \
    core/SystemEntityFactory.cpp \
    core/SystemManager.cpp \
    data_aquisition_dll_system/dad_config_widget/dad_config_widget.cpp \
    data_aquisition_dll_system/tof_daq_specific/shared_memory_descriptor.cpp \
    data_aquisition_dll_system/tof_daq_specific/shared_memory_pointer.cpp \
    data_aquisition_dll_system/tof_daq_specific/tof_daq_dll_tools.cpp \
    data_aquisition_dll_system/tof_daq_specific/tof_daq_dll_worker.cpp \
    data_aquisition_dll_system/data_aquisition_dll_system.cpp \
    data_aquisition_dll_system/data_aquisition_dll_worker.cpp \
    data_aquisition_dll_system/data_aquisition_dll_wrapper.cpp \
    data_tools_dll_system/dtd_config_widget/dtd_config_widget.cpp \
    data_tools_dll_system/data_tools_dll_system.cpp \
    data_tools_dll_system/twtoolswrapper.cpp \
    DataAquisitionSystem/ConfigurationWidget/DataAquisitionConfigurationWidget.cpp \
    DataAquisitionSystem/DaqDllEntity/DaqDllEntity.cpp \
    DataAquisitionSystem/DaqDllEntity/DaqDllEntityFactory.cpp \
    DataAquisitionSystem/DataAquisitionSystem.cpp \
    DataAquisitionSystem/DataAquisitionThread.cpp \
    DataAquisitionSystem/DataAquisitionWorker.cpp \
    gui_system/config_file/config_file.cpp \
    gui_system/gui_system_config_widget/gui_system_config_widget.cpp \
    gui_system/quick_script_widget/quick_script_widget.cpp \
    gui_system/script_editor/console.cpp \
    gui_system/script_editor/highlighter.cpp \
    gui_system/script_editor/script_editor_window.cpp \
    gui_system/script_editor/textedit.cpp \
    gui_system/script_editor/treemodelcompleter.cpp \
    gui_system/settings_dialog/empty_config_widget.cpp \
    gui_system/settings_dialog/settings_dialog.cpp \
    gui_system/tcp_box_view/analog_view/analogview.cpp \
    gui_system/tcp_box_view/analog_view/flowcontrollerview.cpp \
    gui_system/tcp_box_view/digital_view/digital_indicator.cpp \
    gui_system/tcp_box_view/digital_view/digital_view.cpp \
    gui_system/tcp_box_view/digital_view/switch_button.cpp \
    gui_system/tcp_box_view/tcp_box_view.cpp \
    gui_system/gui_system.cpp \
    gui_system/GuiConstantProvider.cpp \
    gui_system/nephele_main_window.cpp \
    power_supply_dll_system/psd_config_widget/psd_config_widget.cpp \
    power_supply_dll_system/power_supply_dll_system.cpp \
    power_supply_dll_system/power_supply_dll_wrapper.cpp \
    script_system/script_config_widget/script_config_widget.cpp \
    script_system/script_handler/script_act_handler.cpp \
    script_system/script_handler/script_daq_handler.cpp \
    script_system/script_handler/script_dialoges_handler.cpp \
    script_system/script_handler/script_load_handler.cpp \
    script_system/script_handler/script_log_handler.cpp \
    script_system/script_handler/script_set_handler.cpp \
    script_system/script_handler/script_util_handler.cpp \
    script_system/script_handler/scriptable_usr_data_handler.cpp \
    script_system/script_system.cpp \
    script_system/scripting_worker.cpp \
    tcp_box_system/JsonRpc/jsonrpcfactory.cpp \
    tcp_box_system/JsonRpc/jsonrpcmethoddescriptor.cpp \
    tcp_box_system/JsonRpc/jsonrpcrequest.cpp \
    tcp_box_system/JsonRpc/jsonrpcresponse.cpp \
    tcp_box_system/JsonRpcDriver/abstractresponseparser.cpp \
    tcp_box_system/JsonRpcDriver/getallmethodsresponseparser.cpp \
    tcp_box_system/JsonRpcDriver/getallresponseparser.cpp \
    tcp_box_system/JsonRpcDriver/JsonRpcBackend.cpp \
    tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.cpp \
    tcp_box_system/JsonRpcDriver/setipresponseparser.cpp \
    tcp_box_system/JsonRpcDriver/setsetpointresponseparser.cpp \
    tcp_box_system/tbs_config_widget/abstractmethodwidget.cpp \
    tcp_box_system/tbs_config_widget/boxconfiguredialog.cpp \
    tcp_box_system/tbs_config_widget/genericmethodwidget.cpp \
    tcp_box_system/tbs_config_widget/tbs_config_widget.cpp \
    tcp_box_system/TcpBoxEntity/TcpBoxEntity.cpp \
    tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.cpp \
    tcp_box_system/abstract_backend.cpp \
    tcp_box_system/abstracttcpdriver.cpp \
    tcp_box_system/arduino_backend.cpp \
    tcp_box_system/flowcontrollerbackend.cpp \
    tcp_box_system/jsonrpcdriver.cpp \
    tcp_box_system/qvmbackend.cpp \
    tcp_box_system/rf_backend.cpp \
    tcp_box_system/tcp_box_system.cpp \
    tcp_box_system/tcp_box_worker.cpp \
    tcp_box_system/tcpcommand.cpp \
    tcp_box_system/tcpcommandqueue.cpp \
    tcp_box_system/tcpdriver.cpp \
    tcp_box_system/JsonRpcDriver/getactresponseparser.cpp \
    DataAquisitionSystem/RegisteredDataEntity/registereddataentity.cpp \
    DataAquisitionSystem/RegisteredDataEntity/registeredsourceentity.cpp \
    DataAquisitionSystem/RegisteredDataEntity/registereddataentityfactory.cpp \
    DataAquisitionSystem/SystemView/registereddataview.cpp \
    gui_system/CustomWidgets/AnalogControl.cpp \
    gui_system/CustomWidgets/AnalogIndicator.cpp \
    gui_system/CustomWidgets/DigitalControl.cpp \
    gui_system/CustomWidgets/DigitalIndicator.cpp \
    gui_system/CustomWidgets/PreassureIndicator.cpp \
    gui_system/CustomWidgets/SpectraPlot.cpp \
    gui_system/CustomWidgets/TimeControl.cpp \
    gui_system/CustomWidgets/TitleLabel.cpp \
    gui_system/CustomWidgets/NepheleDockWidget.cpp \
    gui_system/CustomWidgets/AnalogPlot.cpp \
    tcp_box_system/SystemView/tcpboxview.cpp \
    tcp_box_system/TcpBoxEntity/valueentity.cpp \
    tcp_box_system/TcpBoxEntity/analoginputvalueentity.cpp \
    tcp_box_system/TcpBoxEntity/analogoutputvalueentity.cpp \
    tcp_box_system/TcpBoxEntity/analoginputoutputvalueentity.cpp \
    tcp_box_system/TcpBoxEntity/analogvalueentity.cpp \
    tcp_box_system/TcpBoxEntity/digitalinputvalueentity.cpp \
    tcp_box_system/TcpBoxEntity/digitaloutputvalueentity.cpp \
    tcp_box_system/TcpBoxEntity/digitalinputoutputvalueentity.cpp \
    gui_system/CustomWidgets/digitalcontrolbutton.cpp \
    gui_system/CustomWidgets/digitalcontrollabel.cpp \
    gui_system/CustomWidgets/entitysignalinterface.cpp \
    gui_system/SystemView/actvaluegraph.cpp \
    tcp_box_system/JsonRpcDriver/boxnameresponseparser.cpp \
    script_system/Scriptables/TofDaqScriptable.cpp \
    SerialBoxSystem/SerialBoxSystem.cpp \
    SerialBoxSystem/SerialBoxesThread.cpp \
    SerialBoxSystem/SerialBoxesWorker.cpp \
    SerialBoxSystem/sbs_config_widget/sbs_config_widget.cpp \
    SerialBoxSystem/SerialJsonRpcDriver.cpp \
    SerialBoxSystem/SerialPCUDriver.cpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPRequest.cpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPResponse.cpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPFactory.cpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPBackend.cpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPTelegramm.cpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPParameter.cpp

HEADERS  += \
    core/abstract_configuration_widget.hpp \
    core/abstract_scriptable_object.hpp \
    core/abstract_system.hpp \
    core/AbstractAction.hpp \
    core/AbstractSystemView.hpp \
    core/EntityManager.hpp \
    core/EventManager.hpp \
    core/ManagerLocator.hpp \
    core/StaticComponentId.hpp \
    core/SystemEntity.hpp \
    core/SystemEntityFactory.hpp \
    core/SystemManager.hpp \
    core/SystemState.hpp \
    core/systemviewscontainer.h \
    data_aquisition_dll_system/dad_config_widget/dad_config_widget.hpp \
    data_aquisition_dll_system/tof_daq_specific/shared_memory_descriptor.h \
    data_aquisition_dll_system/tof_daq_specific/shared_memory_pointer.h \
    data_aquisition_dll_system/tof_daq_specific/tof_daq_dll_tools.hpp \
    data_aquisition_dll_system/tof_daq_specific/tof_daq_dll_worker.hpp \
    data_aquisition_dll_system/data_aquisition_dll_system.hpp \
    data_aquisition_dll_system/data_aquisition_dll_worker.hpp \
    data_aquisition_dll_system/data_aquisition_dll_wrapper.hpp \
    data_tools_dll_system/dtd_config_widget/dtd_config_widget.hpp \
    data_tools_dll_system/data_tools_dll_system.hpp \
    data_tools_dll_system/twtoolswrapper.h \
    DataAquisitionSystem/ConfigurationWidget/DataAquisitionConfigurationWidget.hpp \
    DataAquisitionSystem/DaqDllEntity/DaqDllEntity.hpp \
    DataAquisitionSystem/DaqDllEntity/DaqDllEntityFactory.hpp \
    DataAquisitionSystem/DaqDllEntity/DaqDllEntityState.hpp \
    DataAquisitionSystem/DataAquisitionSystem.hpp \
    DataAquisitionSystem/DataAquisitionThread.hpp \
    DataAquisitionSystem/DataAquisitionWorker.hpp \
    gui_system/config_file/config_file.hpp \
    gui_system/gui_system_config_widget/gui_system_config_widget.hpp \
    gui_system/quick_script_widget/quick_script_widget.hpp \
    gui_system/script_editor/console.h \
    gui_system/script_editor/highlighter.h \
    gui_system/script_editor/script_editor_window.hpp \
    gui_system/script_editor/textedit.h \
    gui_system/script_editor/treemodelcompleter.h \
    gui_system/settings_dialog/empty_config_widget.hpp \
    gui_system/settings_dialog/settings_dialog.hpp \
    gui_system/tcp_box_view/analog_view/analogview.hpp \
    gui_system/tcp_box_view/analog_view/flowcontrollerview.hpp \
    gui_system/tcp_box_view/digital_view/digital_indicator.hpp \
    gui_system/tcp_box_view/digital_view/digital_view.hpp \
    gui_system/tcp_box_view/digital_view/switch_button.hpp \
    gui_system/tcp_box_view/tcp_box_view.hpp \
    gui_system/gui_system.hpp \
    gui_system/GuiConstantProvider.hpp \
    gui_system/nephele_main_window.hpp \
    power_supply_dll_system/psd_config_widget/psd_config_widget.hpp \
    power_supply_dll_system/power_supply_dll_system.hpp \
    power_supply_dll_system/power_supply_dll_wrapper.h \
    script_system/script_config_widget/script_config_widget.hpp \
    script_system/script_handler/script_act_handler.hpp \
    script_system/script_handler/script_daq_handler.hpp \
    script_system/script_handler/script_dialoges_handler.hpp \
    script_system/script_handler/script_load_handler.hpp \
    script_system/script_handler/script_log_handler.hpp \
    script_system/script_handler/script_set_handler.hpp \
    script_system/script_handler/script_util_handler.hpp \
    script_system/script_handler/scriptable_usr_data_handler.hpp \
    script_system/script_system.hpp \
    script_system/scripting_worker.hpp \
    tcp_box_system/JsonRpc/JrpcDatatype.hpp \
    tcp_box_system/JsonRpc/jsonrpcfactory.hpp \
    tcp_box_system/JsonRpc/jsonrpcmethoddescriptor.hpp \
    tcp_box_system/JsonRpc/jsonrpcrequest.hpp \
    tcp_box_system/JsonRpc/jsonrpcresponse.hpp \
    tcp_box_system/JsonRpcDriver/abstractresponseparser.hpp \
    tcp_box_system/JsonRpcDriver/getallmethodsresponseparser.hpp \
    tcp_box_system/JsonRpcDriver/getallresponseparser.hpp \
    tcp_box_system/JsonRpcDriver/JsonRpcBackend.hpp \
    tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp \
    tcp_box_system/JsonRpcDriver/setipresponseparser.hpp \
    tcp_box_system/JsonRpcDriver/setsetpointresponseparser.hpp \
    tcp_box_system/tbs_config_widget/abstractmethodwidget.hpp \
    tcp_box_system/tbs_config_widget/boxconfiguredialog.hpp \
    tcp_box_system/tbs_config_widget/genericmethodwidget.hpp \
    tcp_box_system/tbs_config_widget/tbs_config_widget.hpp \
    tcp_box_system/TcpBoxEntity/TcpBoxEntity.hpp \
    tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp \
    tcp_box_system/abstract_backend.hpp \
    tcp_box_system/abstracttcpdriver.hpp \
    tcp_box_system/arduino_backend.hpp \
    tcp_box_system/backend_type.hpp \
    tcp_box_system/flowcontrollerbackend.h \
    tcp_box_system/jsonrpcdriver.hpp \
    tcp_box_system/qvmbackend.hpp \
    tcp_box_system/rf_backend.hpp \
    tcp_box_system/tcp_box_system.hpp \
    tcp_box_system/tcp_box_worker.hpp \
    tcp_box_system/tcpcommand.hpp \
    tcp_box_system/tcpcommandqueue.hpp \
    tcp_box_system/tcpdriver.hpp \
    tcp_box_system/tcpdriverdatatypes.hpp \
    tcp_box_system/JsonRpcDriver/getactresponseparser.hpp \
    DataAquisitionSystem/RegisteredDataEntity/registereddataentity.hpp \
    DataAquisitionSystem/RegisteredDataEntity/registeredsourceentity.hpp \
    DataAquisitionSystem/RegisteredDataEntity/registereddataentityfactory.hpp \
    DataAquisitionSystem/SystemView/registereddataview.hpp \
    gui_system/CustomWidgets/AnalogControl.hpp \
    gui_system/CustomWidgets/AnalogIndicator.hpp \
    gui_system/CustomWidgets/DigitalControl.hpp \
    gui_system/CustomWidgets/DigitalIndicator.hpp \
    gui_system/CustomWidgets/PreassureIndicator.hpp \
    gui_system/CustomWidgets/SpectraPlot.hpp \
    gui_system/CustomWidgets/TimeControl.hpp \
    gui_system/CustomWidgets/TitleLabel.hpp \
    gui_system/CustomWidgets/NepheleDockWidget.hpp \
    gui_system/CustomWidgets/AnalogPlot.hpp \
    tcp_box_system/SystemView/tcpboxview.hpp \
    tcp_box_system/TcpBoxEntity/valueentity.hpp \
    tcp_box_system/TcpBoxEntity/analoginputvalueentity.hpp \
    tcp_box_system/TcpBoxEntity/analogoutputvalueentity.hpp \
    tcp_box_system/TcpBoxEntity/analoginputoutputvalueentity.hpp \
    tcp_box_system/TcpBoxEntity/analogvalueentity.hpp \
    tcp_box_system/TcpBoxEntity/digitalinputvalueentity.hpp \
    tcp_box_system/TcpBoxEntity/digitaloutputvalueentity.hpp \
    tcp_box_system/TcpBoxEntity/digitalinputoutputvalueentity.hpp \
    gui_system/CustomWidgets/digitalcontrolbutton.hpp \
    gui_system/CustomWidgets/digitalcontrollabel.hpp \
    gui_system/CustomWidgets/entitysignalinterface.hpp \
    gui_system/SystemView/actvaluegraph.hpp \
    tcp_box_system/JsonRpcDriver/boxnameresponseparser.hpp \
    script_system/Scriptables/TofDaqScriptable.hpp \
    SerialBoxSystem/SerialBoxSystem.hpp \
    SerialBoxSystem/SerialBoxesThread.hpp \
    SerialBoxSystem/SerialBoxesWorker.hpp \
    SerialBoxSystem/sbs_config_widget/sbs_config_widget.hpp \
    SerialBoxSystem/SerialJsonRpcDriver.hpp \
    SerialBoxSystem/SerialPCUDriver.hpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPRequest.hpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPResponse.hpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPFactory.hpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPBackend.hpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPTelegramm.hpp \
    SerialBoxSystem/PfeifferVacuumProtocol/PVPParameter.hpp


unix {
    target.path = /usr/lib
    INSTALLS += target
}



## ================
INCLUDEPATH += $$PWD/../../ext/TofDaq/include
DEPENDPATH += $$PWD/../../ext/TofDaq/include
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/release/ -lqtofdaqdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/debug/ -lqtofdaqdll
else:unix:!macx: LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/ -lqtofdaqdll
else:macx: LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/ -lqtofdaqdll

INCLUDEPATH += $$PWD/../libs/qtofdaqdll
DEPENDPATH += $$PWD/../libs/qtofdaqdll

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/release/qtofdaqdll.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/debug/qtofdaqdll.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/libqtofdaqdll.a
else:macx: PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/libqtofdaqdll.a
## ================

### ================
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplot2
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug -lqcustomplotd2
#else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplot
#else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

#INCLUDEPATH += $$PWD/../../ext/qcustomplot
#DEPENDPATH += $$PWD/../../ext/qcustomplot
#unix | win32-g++:{
#    QMAKE_CXXFLAGS += -isystem $$PWD/../../ext/qcustomplot
#}

##win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/qcustomplotd2.a
### ================


#### ================
#DEFINES += OLD_QCUSTOMPLOT
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot_old/release/ -lqcustomplot_old
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot_old/debug/ -lqcustomplot_old

#INCLUDEPATH += $$PWD/../../ext/qcustomplot_old
#DEPENDPATH += $$PWD/../../ext/qcustomplot_old

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot_old/release/libqcustomplot_old.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot_old/debug/libqcustomplot_old.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot_old/release/qcustomplot_old.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot_old/debug/qcustomplot_old.lib
#### ================


#### ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-staticlib/release/ -lqcustomplot-staticlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-staticlib/debug/ -lqcustomplot-staticlib

INCLUDEPATH += $$PWD/../../ext/qcustomplot/qcustomplot-staticlib
DEPENDPATH += $$PWD/../../ext/qcustomplot/qcustomplot-staticlib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot/qcustomplot-staticlib/release/libqcustomplot-staticlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot/qcustomplot-staticlib/debug/libqcustomplot-staticlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot/qcustomplot-staticlib/release/qcustomplot-staticlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ext/qcustomplot/qcustomplot-staticlib/debug/qcustomplot-staticlib.lib
#### ================
