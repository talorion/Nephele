#ifndef TALORION_ABSTRACT_CONFIGURATION_WIDGET_HPP
#define TALORION_ABSTRACT_CONFIGURATION_WIDGET_HPP

#include <QWidget>


namespace talorion {

    class abstract_configuration_widget : public QWidget
    {
        Q_OBJECT
    public:
        explicit abstract_configuration_widget(QWidget *par = 0): QWidget(par){}
        virtual ~abstract_configuration_widget(){}
        Q_DISABLE_COPY(abstract_configuration_widget)

        void refresh_data(){return do_refresh_data();}

    signals:

    public slots:

    private:
        virtual void do_refresh_data()=0;
    };

} // namespace talorion

#endif // TALORION_ABSTRACT_CONFIGURATION_WIDGET_HPP
