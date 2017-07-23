#ifndef ANALOGVIEW_H
#define ANALOGVIEW_H

#include <QWidget>


namespace talorion {

    class analogView : public QWidget
    {
        Q_OBJECT
    public:
        typedef enum mode_{
            Input = 0x0001,
            Output = 0x0002,
            InputOutput = Input | Output
        }mode;

    public:
        analogView(int entity=0,mode md= InputOutput, QWidget *par = 0);
        ~analogView(void);
        Q_DISABLE_COPY(analogView)

        int entity() const;

        virtual int getLableWidth()const {return 0;}
        virtual void setLabelMinimumWidth(int ) {}


    signals:
        void change_set_value(int m_entity, double value);

    public slots:
        virtual void changeActValue(double actValue) = 0;
        virtual void changeSetValue(double setValue) = 0;
        virtual int updateLabelMinimumWidth(int ) {return 0;}

    protected:
        //analogValue* data;
        const int m_entity;
        mode m_mode;

    };
}

#endif // ANALOGVIEW_H
