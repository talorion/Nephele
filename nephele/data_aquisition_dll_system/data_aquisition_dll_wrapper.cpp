#include "data_aquisition_dll_wrapper.hpp"

#include <QDebug>

#include "core/event_manager.hpp"

namespace talorion{
    data_aquisition_dll_wrapper::data_aquisition_dll_wrapper(QObject *par) :
        QObject(par),
        m_InitializeDll(NULL),
        m_CleanupDll(NULL),
        m_StartAcquisition(NULL),
        m_StopAcquisition(NULL),
        m_AquisitionActive(NULL),
        m_RegisterUserDat(NULL),
        m_UnregisterUserData(NULL),
        m_data_aquisition_dll(NULL),
        m_success_return(0),
        registered_user_data()
    {
        connect(event_manager::get_instance(),SIGNAL(start_aquisition()),this,SLOT(start_aquisition()));
        connect(event_manager::get_instance(),SIGNAL(stop_aquisition()),this,SLOT(stop_aquisition()));
    }

    data_aquisition_dll_wrapper::~data_aquisition_dll_wrapper()
    {

    }

    void data_aquisition_dll_wrapper::init()
    {
        QString dll_name = "TofDaqDll.dll";
        m_success_return = 4;

        m_data_aquisition_dll = new QLibrary(dll_name);
        if (!m_data_aquisition_dll->load()){
            qDebug()<<m_data_aquisition_dll->errorString();
            return;
        }

        QString meth = "TwInitializeDll";
        m_InitializeDll = resolve_method<InitializeDll_prototype>(meth);

        meth = "TwCleanupDll";
        m_CleanupDll= resolve_method<CleanupDll_prototype>(meth);

        meth = "TwStartAcquisition";
        m_StartAcquisition= resolve_method<StartAcquisition_prototype>(meth);

        meth = "TwStopAcquisition";
        m_StopAcquisition= resolve_method<StopAcquisition_prototype>(meth);

        meth = "TwDaqActive";
        m_AquisitionActive= resolve_method<DaqActive_prototype>(meth);

        meth = "TwRegisterUserDataBuf";
        m_RegisterUserDat= resolve_method<RegisterUserData_prototype>(meth);

        meth = "TwUnregisterUserData";
        m_UnregisterUserData= resolve_method<UnregisterUserData_prototype>(meth);

        initialize_dll();

        QStringList value_names;
        value_names<<"val1"<<"val2"<<"val3"<<"val4";
        register_user_data(value_names);
    }

    void data_aquisition_dll_wrapper::dispose()
    {
        unregister_all_user_data();

        cleanup_dll();
    }

    void data_aquisition_dll_wrapper::initialize_dll()
    {
        if(m_InitializeDll){
            qDebug()<<"m_InitializeDll";
            m_InitializeDll();
        }
    }

    void data_aquisition_dll_wrapper::cleanup_dll()
    {
        if(m_CleanupDll){
            qDebug()<<"m_CleanupDll";
            m_CleanupDll();
        }
    }

    void data_aquisition_dll_wrapper::unregister_all_user_data()
    {
        foreach (QString path, registered_user_data) {
            unregister_user_data(path);
        }
    }

    void data_aquisition_dll_wrapper::start_aquisition()
    {
        if(m_StartAcquisition){
            qDebug()<<"m_StartAcquisition";
            m_StartAcquisition();
        }
    }

    void data_aquisition_dll_wrapper::stop_aquisition()
    {
        if(m_StopAcquisition){
            qDebug()<<"m_StopAcquisition";
            m_StopAcquisition();
        }
    }

    bool data_aquisition_dll_wrapper::aquisition_active()
    {
        if(m_AquisitionActive){
            qDebug()<<"m_DaqActive";
            return m_AquisitionActive();
        }
        return false;
    }

    void data_aquisition_dll_wrapper::register_user_data(const QStringList& value_names, const QString& path , int cmp_lvl)
    {
        if(m_RegisterUserDat){
            qDebug()<<"m_RegisterUserDat";
            int NbrElements = value_names.size();
            QByteArray path_ba = path.toLocal8Bit();
            QByteArray desc_ba;
            QByteArray desc_elem_ba;
            foreach (QString des_elem, value_names) {
                desc_elem_ba = des_elem.toLocal8Bit();
                desc_elem_ba.resize(256);
                desc_ba.append(desc_elem_ba);
                desc_elem_ba.clear();
            }

            if(success(m_RegisterUserDat(path_ba.data(), NbrElements, desc_ba.data(), cmp_lvl))){
                registered_user_data.append(path);
            }
        }

    }

    void data_aquisition_dll_wrapper::unregister_user_data(const QString& path)
    {
        //QString path = "/USER_DATA";
        QByteArray path_ba = path.toLocal8Bit();

        if(m_UnregisterUserData){
            qDebug()<<"m_UnregisterUserData";
            if(success(m_UnregisterUserData(path_ba.data()))){
                registered_user_data.removeAll(path);
            }
        }
    }
}
