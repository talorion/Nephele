#ifndef SHARED_MEMORY_DESCRIPTOR_HPP
#define SHARED_MEMORY_DESCRIPTOR_HPP


#include <QtCore>
#include <QObject>

#ifndef Q_OS_WIN
typedef qint64 __int64;
//typedef quint64 __int64;
#ifdef TOFDAQDLL_EXPORTS
#define TOFWERK_DAQ_API Q_DECL_EXPORT
#else
#define TOFWERK_DAQ_API Q_DECL_IMPORT
#endif
#endif
//#include <TofDaqDll.h>


namespace talorion {

    class shared_memory_descriptor:public QObject
    {
        Q_OBJECT
    public:
        shared_memory_descriptor(QObject *par = 0);
        ~shared_memory_descriptor();
        Q_DISABLE_COPY(shared_memory_descriptor)

        //TSharedMemoryDesc* current_data()const{return m_current_data;}
        //TSharedMemoryDesc* next_data()const{return m_next_data;}

        //bool is_null()const{return m_current_data==0;}

        void swap();

    private:
        void init();
        void cleanup();

    private:
        //TSharedMemoryDesc* m_data;
        //TSharedMemoryDesc* m_current_data;
        //TSharedMemoryDesc* m_next_data;
    };
    //Q_DECLARE_METATYPE(shared_memory_descriptor)
}

#endif // SHARED_MEMORY_DESCRIPTOR_HPP
