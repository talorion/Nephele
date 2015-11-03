#ifndef TALORION_CONFIG_FILE_HPP
#define TALORION_CONFIG_FILE_HPP

#include "abstract_scriptable_object.hpp"

namespace talorion {

    class config_file: public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        config_file(QObject *par = 0);
        ~config_file();
        Q_DISABLE_COPY(config_file)

        Q_INVOKABLE void loadFile(const QString &fileName);
        Q_INVOKABLE bool saveFile(const QString &fileName);

    signals:
        void change_set_value(int m_entity, double value);

    private:
        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;
    };

} // namespace talorion

#endif // TALORION_CONFIG_FILE_HPP
