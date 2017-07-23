
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

QT_BEGIN_NAMESPACE
class QCompleter;
QT_END_NAMESPACE

namespace talorion{
    class TextEdit : public QTextEdit
    {
        Q_OBJECT

    public:
        TextEdit(QWidget *par = 0);
        ~TextEdit();
        Q_DISABLE_COPY(TextEdit)

        void setCompleter(QCompleter *comp);
        QCompleter *completer() const;

    protected:
        void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
        void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

    private slots:
        void insertCompletion(const QString &completion);

    private:
        QString textUnderCursor();

    private:
        QCompleter *m_com;
    };
}

#endif // TEXTEDIT_H

