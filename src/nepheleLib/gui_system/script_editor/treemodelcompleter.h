
#ifndef TREEMODELCOMPLETER_H
#define TREEMODELCOMPLETER_H

#include <QCompleter>

namespace talorion{
    class TreeModelCompleter : public QCompleter
    {
        Q_OBJECT
        Q_PROPERTY(QString separator READ separator WRITE setSeparator)

    public:
        explicit TreeModelCompleter(QObject *par = 0);
        explicit TreeModelCompleter(QAbstractItemModel *m, QObject *par = 0);
        ~TreeModelCompleter();
        Q_DISABLE_COPY(TreeModelCompleter)


        QString separator() const;
    public slots:
        void setSeparator(const QString &sep);

    protected:
        QStringList splitPath(const QString &path) const Q_DECL_OVERRIDE;
        QString pathFromIndex(const QModelIndex &index) const Q_DECL_OVERRIDE;

    private:
        QString seperator;
    };
}

#endif // TREEMODELCOMPLETER_H

