#include "shared_memory_descriptor.h"


namespace talorion{
    shared_memory_descriptor::shared_memory_descriptor(QObject *par):
        QObject(par),
        //m_data(0)
        m_current_data(NULL),
        m_next_data(NULL)
    {
        init();
    }

    shared_memory_descriptor::~shared_memory_descriptor()
    {
cleanup();
    }

    void shared_memory_descriptor::swap()
    {
        TSharedMemoryDesc* temp    = m_current_data;
        m_current_data                 = m_next_data;
        m_next_data                    = temp;
    }

    void shared_memory_descriptor::init()
    {
        //m_data = new TSharedMemoryDesc();
        m_current_data = new TSharedMemoryDesc();
        m_next_data = new TSharedMemoryDesc();
    }

    void shared_memory_descriptor::cleanup()
    {
        delete m_current_data;
        delete m_next_data;
    }
}

