#ifndef TALORION_ABSTRACT_CONFIGURATION_WIDGET_HPP
#define TALORION_ABSTRACT_CONFIGURATION_WIDGET_HPP

#include <QWidget>


namespace talorion {

    class abstract_configuration_widget : public QWidget
    {
        Q_OBJECT
    public:
        explicit abstract_configuration_widget(QWidget *par = 0);
        virtual ~abstract_configuration_widget();
        Q_DISABLE_COPY(abstract_configuration_widget)

    signals:

    public slots:

    private:
    };

} // namespace talorion

#endif // TALORION_ABSTRACT_CONFIGURATION_WIDGET_HPP
