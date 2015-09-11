#ifndef TALORION_ABSTRACT_SCRIPTABLE_OBJECT_HPP
#define TALORION_ABSTRACT_SCRIPTABLE_OBJECT_HPP

#include <QObject>

QT_BEGIN_NAMESPACE
class QScriptValue;
QT_END_NAMESPACE


namespace talorion {

    class abstract_scriptable_object : public QObject
    {
        Q_OBJECT
    public:
        explicit abstract_scriptable_object(QString scrpt_name, QObject *parent = 0);
        virtual ~abstract_scriptable_object();
        Q_DISABLE_COPY(abstract_scriptable_object)

        QString script_name() const;

        QScriptValue *getScrip_value() const;
        void setScrip_value(QScriptValue *value);

    signals:
        void script_finished();

    public slots:

    private:
        const QString m_script_name;
        QScriptValue* scrip_value;
    };

} // namespace talorion

#endif // TALORION_ABSTRACT_SCRIPTABLE_OBJECT_HPP
