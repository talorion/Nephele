#ifndef SHARED_MEMORY_POINTER_H
#define SHARED_MEMORY_POINTER_H

#include <QObject>
#include <QtCore>
#include <QtGlobal>

#if defined( Q_OS_WIN )
//#if defined( _MSC_VER )
#include <WTypes.h>
#else
#define __int64 long long
#define __stdcall
#define __declspec( dllexport )
#endif

#include "TofDaqDll.h"

namespace talorion{
    class shared_memory_pointer: public QObject
    {
        Q_OBJECT
    public:
        shared_memory_pointer(QObject *par = 0);
        shared_memory_pointer(int NbrSamp, int NbrPea, int NbrSeg, int NbrBu, int NbrWri, QObject *par = 0);
        ~shared_memory_pointer();
        Q_DISABLE_COPY(shared_memory_pointer)

        //TSharedMemoryPointer* data()const{return m_data;}
        TSharedMemoryPointer* current_data()const{return m_current_data;}
        TSharedMemoryPointer* next_data()const{return m_next_data;}

        bool is_null()const{return m_current_data==0;}

        int NbrSamples() const;

        void swap();

    private:
        void init(TSharedMemoryPointer* m_data, int NbrSamples, int NbrPeaks, int NbrSegments, int NbrBufs, int NbrWrites);
        void cleanup(TSharedMemoryPointer* dat, int NbrBufs);

    private:
        //TSharedMemoryPointer* m_data;
        TSharedMemoryPointer* m_current_data;
        TSharedMemoryPointer* m_next_data;
        const int m_NbrSamples;
        const int m_NbrPeaks;
        const int m_NbrSegments;
        const int m_NbrBufs;
        const int m_NbrWrites;
    };
    //Q_DECLARE_METATYPE(shared_memory_pointer)
}

#endif // SHARED_MEMORY_POINTER_H
