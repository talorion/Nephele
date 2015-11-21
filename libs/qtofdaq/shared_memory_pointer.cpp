#include "shared_memory_pointer.hpp"


namespace talorion{
    shared_memory_pointer::shared_memory_pointer(QObject *par):
        QObject(par),
        //m_data(0),
        //m_current_data(NULL),
        //m_next_data(NULL),
        m_NbrSamples(0),
        m_NbrPeaks(0),
        m_NbrSegments(0),
        m_NbrBufs(0),
        m_NbrWrites(0)
    {

    }

    //    shared_memory_pointer::shared_memory_pointer(int NbrSamp, int NbrPea, int NbrSeg, int NbrBu, int NbrWri, QObject *par):
    //        QObject(par),
    //        //m_data(0),
    //        m_current_data(NULL),
    //        m_next_data(NULL),
    //        m_NbrSamples(NbrSamp),
    //        m_NbrPeaks(NbrPea),
    //        m_NbrSegments(NbrSeg),
    //        m_NbrBufs(NbrBu),
    //        m_NbrWrites(NbrWri)
    //    {
    //        //init(m_current_data, NbrSamp, NbrPea, NbrSeg, NbrBu, NbrWri);
    //        //init(m_next_data, NbrSamp, NbrPea, NbrSeg, NbrBu, NbrWri);
    //    }

    shared_memory_pointer::~shared_memory_pointer()
    {
        //cleanup(m_current_data, m_NbrBufs);
        //cleanup(m_next_data, m_NbrBufs);
    }

//    void shared_memory_pointer::cleanup(TSharedMemoryPointer *dat, int NbrBufs)
//    {
//        if(dat->SumSpectrum){
//            delete[] dat->SumSpectrum;
//            dat->SumSpectrum = 0;
//        }

//        if(dat->SumSpectrum2){
//            delete[] dat->SumSpectrum2;
//            dat->SumSpectrum2 = 0;
//        }

//        if(dat->TofData){
//            for(int i=0; i<NbrBufs;++i){delete[] dat->TofData[i];dat->TofData[i]=0;}
//            delete[] dat->TofData;
//            dat->TofData = 0;
//        }

//        if(dat->TofData2){
//            for(int i=0; i<NbrBufs;++i){delete[] dat->TofData2[i];dat->TofData2[i]=0;}
//            delete[] dat->TofData2;
//            dat->TofData2 = 0;
//        }


//        if(dat->PeakData){
//            delete[] dat->PeakData;
//            dat->PeakData = 0;
//        }

//        if(dat->PeakData2){
//            delete[] dat->PeakData2;
//            dat->PeakData2 = 0;
//        }

//        if(dat->Timing){
//            delete[] dat->Timing;
//            dat->Timing = 0;
//        }

//        delete dat;
//        dat = 0;
//    }

    int shared_memory_pointer::NbrSamples() const
    {
        return m_NbrSamples;
    }

    void shared_memory_pointer::swap()
    {
        //TSharedMemoryPointer* temp    = m_current_data;
        //m_current_data                 = m_next_data;
        //m_next_data                    = temp;
    }


    //    void shared_memory_pointer::init(TSharedMemoryPointer *m_data, int NbrSamp, int NbrPeaks, int NbrSegments, int NbrBufs, int NbrWrites)
    //    {
    //        m_data = new TSharedMemoryPointer();

    ////        m_data->SumSpectrum     = new double[NbrSamp];
    ////        m_data->SumSpectrum2    = new double[NbrSamp];

    ////        m_data->TofData         = new float*[NbrBufs];
    ////        m_data->TofData2         = new float*[NbrBufs];
    ////        for(int i=0; i<NbrBufs; ++i){
    ////            m_data->TofData[i] = new float[NbrSamp*NbrSegments];
    ////            m_data->TofData2[i] = new float[NbrSamp*NbrSegments];
    ////        }

    ////        m_data->PeakData        = new float[NbrPeaks*NbrSegments*NbrBufs];
    ////        m_data->PeakData2       = new float[NbrPeaks*NbrSegments*NbrBufs];

    ////        m_data->Timing          = new double[NbrBufs*NbrWrites];

    ////        m_data->RawData32Ch1    = 0;
    ////        m_data->RawData32Ch2    = 0;

    ////        m_data->RawData16Ch1    = 0;
    ////        m_data->RawData16Ch2    = 0;

    ////        m_data->RawData8Ch1     = 0;
    ////        m_data->RawData8Ch2     = 0;
    //    }
}
