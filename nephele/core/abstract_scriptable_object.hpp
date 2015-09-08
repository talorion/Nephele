#ifndef TALORION_ABSTRACT_SCRIPTABLE_OBJECT_HPP
#define TALORION_ABSTRACT_SCRIPTABLE_OBJECT_HPP

#include <QObject>

namespace talorion {

    class abstract_scriptable_object : public QObject
    {
        Q_OBJECT
    public:
        explicit abstract_scriptable_object(QString scrpt_name, QObject *parent = 0);
        virtual ~abstract_scriptable_object();
        Q_DISABLE_COPY(abstract_scriptable_object)

        QString script_name() const;

    signals:

    public slots:

    private:
        const QString m_script_name;
    };

} // namespace talorion

#endif // TALORION_ABSTRACT_SCRIPTABLE_OBJECT_HPP
