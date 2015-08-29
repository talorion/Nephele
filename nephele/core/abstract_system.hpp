#ifndef ABSTRACT_SYSTEM
#define ABSTRACT_SYSTEM

namespace talorion{
    class abstract_system
    {
    public:
        abstract_system(){}
        virtual ~abstract_system(){}
        Q_DISABLE_COPY(abstract_system)

        void start_system(){return do_start_system();}

    private:
        virtual void do_start_system()=0;
    };
}

#endif // ABSTRACT_SYSTEM

