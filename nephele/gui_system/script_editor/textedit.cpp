#include "textedit.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

namespace talorion{
    TextEdit::TextEdit(QWidget *par)
        : QTextEdit(par), m_com(0)
    {
        //        setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
        //                        " 3 characters. You can trigger autocompletion using ") +
        //                     QKeySequence(Qt::CTRL + Qt::Key_E).toString(QKeySequence::NativeText));
    }

    TextEdit::~TextEdit()
    {
    }

    void TextEdit::setCompleter(QCompleter *comp)
    {
        if (m_com)
            QObject::disconnect(m_com, 0, this, 0);

        m_com = comp;

        if (!m_com)
            return;

        m_com->setWidget(this);
        m_com->setCompletionMode(QCompleter::PopupCompletion);
        m_com->setCaseSensitivity(Qt::CaseInsensitive);
        //m_com->setMaxVisibleItems(5);
        QObject::connect(m_com, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    }

    QCompleter *TextEdit::completer() const
    {
        return m_com;
    }

    void TextEdit::insertCompletion(const QString& completion)
    {
        if (m_com->widget() != this){

            return;
        }
        QTextCursor tc = textCursor();
        int extra = completion.length() - m_com->completionPrefix().length();
        tc.movePosition(QTextCursor::Left);
        tc.movePosition(QTextCursor::EndOfWord);
        tc.insertText(completion.right(extra));
        setTextCursor(tc);

        document()->setModified(true);
    }

    QString TextEdit::textUnderCursor()
    {
        //QTextCursor tc = textCursor();
        //tc.select(QTextCursor::WordUnderCursor);
        //tc.selectionStart()
        //return tc.selectedText();
        QTextCursor tc = textCursor();
        tc.select(QTextCursor::LineUnderCursor);
        //tc.setPosition(tc.position()-5);
        //tc.setPosition(tc.selectionStart() -1 , QTextCursor::KeepAnchor);
        //setTextCursor(tc);

        //tc.movePosition(QTextCursor::StartOfWord);

        //tc.movePosition(QTextCursor::PreviousCharacter);
        //QStringList splitted = tc.selectedText().split(" ;{}():",QString::SkipEmptyParts);
        QStringList splitted = tc.selectedText().split(QRegExp("[\\s;:\\{\\{\\[\\]]"),QString::SkipEmptyParts);
        QString ret = "";
        if(!splitted.isEmpty())
            ret = splitted.last();
        return ret.trimmed();
    }

    void TextEdit::focusInEvent(QFocusEvent *e)
    {
        if (m_com)
            m_com->setWidget(this);
        QTextEdit::focusInEvent(e);
    }

    void TextEdit::keyPressEvent(QKeyEvent *e)
    {
        if (m_com && m_com->popup()->isVisible()) {
            // The following keys are forwarded by the completer to the widget
            switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return; // let the completer do default behavior
            default:
                break;
            }
        }

        bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
        if (!m_com || !isShortcut){ // do not process the shortcut when we have a completer
            QTextEdit::keyPressEvent(e);
        }

        const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
        if (!m_com || (ctrlOrShift && e->text().isEmpty())){
            return;
        }

        static QString eow("~!@#$%^&*()_+{}|:\"<>?,/;'[]\\-="); // end of word
        bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();

        if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1 || eow.contains(e->text().right(3)))) {
            m_com->popup()->hide();
            return;
        }

        if (completionPrefix != m_com->completionPrefix()) {
            //qDebug()<<completionPrefix;
            m_com->setCompletionPrefix(completionPrefix);
            m_com->popup()->setCurrentIndex(m_com->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(m_com->popup()->sizeHintForColumn(0) + m_com->popup()->verticalScrollBar()->sizeHint().width());
        m_com->complete(cr); // popup it up!

    }
}

