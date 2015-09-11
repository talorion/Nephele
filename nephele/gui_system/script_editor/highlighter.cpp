/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "highlighter.h"

namespace talorion{
    Highlighter::Highlighter(QTextDocument *par)
        : QSyntaxHighlighter(par),
          highlightingRules(),
          commentStartExpression(),
          commentEndExpression(),
          keywordFormat(),
          datatypesFormat(),
          extensionFormat(),
          classFormat(),
          singleLineCommentFormat(),
          multiLineCommentFormat(),
          quotationFormat(),
          functionFormat()
    {
        HighlightingRule rule;

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
        rule.format = functionFormat;
        highlightingRules.append(rule);

        keywordFormat.setForeground(Qt::darkYellow);
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns;
        keywordPatterns << "\\bdo\\b"
                        << "\\bif\\b"
                        << "\\bin\\b"
                        << "\\bfor\\b"
                        << "\\blet\\b"
                        << "\\bnew\\b"
                        << "\\btry\\b"
                        << "\\bvar\\b"
                        << "\\bcase\\b"
                        << "\\belse\\b"
                        << "\\benum\\b"
                        << "\\beval\\b"
                        << "\\bnull\\b"
                        << "\\bthis\\b"
                        << "\\btrue\\b"
                        << "\\bvoid\\b"
                        << "\\bwith\\b"
                        << "\\bawait\\b"
                        << "\\bbreak\\b"
                        << "\\bcatch\\b"
                        << "\\bclass\\b"
                        << "\\bconst\\b"
                        << "\\bfalse\\b"
                        << "\\bsuper\\b"
                        << "\\bthrow\\b"
                        << "\\bwhile\\b"
                        << "\\byield\\b"
                        << "\\bdelete\\b"
                        << "\\bexport\\b"
                        << "\\bimport\\b"
                        << "\\bpublic\\b"
                        << "\\breturn\\b"
                        << "\\bstatic\\b"
                        << "\\bswitch\\b"
                        << "\\btypeof\\b"
                        << "\\bdefault\\b"
                        << "\\bextends\\b"
                        << "\\bfinally\\b"
                        << "\\bpackage\\b"
                        << "\\bprivate\\b"
                        << "\\bcontinue\\b"
                        << "\\bdebugger\\b"
                        << "\\bfunction\\b"
                        << "\\barguments\\b"
                        << "\\binterface\\b"
                        << "\\bprotected\\b"
                        << "\\bimplements\\b"
                        << "\\binstanceof\\b";

        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        datatypesFormat.setForeground(Qt::darkBlue);
        datatypesFormat.setFontWeight(QFont::Bold);
        QStringList dataTypePatterns;
        dataTypePatterns << "\\bBoolean\\b"
                         << "\\bNull\\b"
                         << "\\bUndefined\\b"
                         << "\\bNumber\\b"
                         << "\\bString\\b"
                         << "\\bSymbol\\b"
                         << "\\bObject\\b";
        foreach (const QString &pattern, dataTypePatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = datatypesFormat;
            highlightingRules.append(rule);
        }

        extensionFormat.setForeground(Qt::darkYellow);
        extensionFormat.setFontWeight(QFont::Bold);
        QStringList extensionPatterns;
        extensionPatterns   << "\\b__proto__\\b"
                            << "\\b__defineGetter__\\b"
                            << "\\b__defineSetter__\\b"
                            << "\\bconnect\\b"
                            << "\\bdisconnect\\b"
                            << "\\bfindChild\\b"
                            << "\\btoString\\b"
                            << "\\bfindChildren\\b"
                            << "\\bgc\\b"
                            << "\\bbacktrace\\b";
        foreach (const QString &pattern, extensionPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = extensionFormat;
            highlightingRules.append(rule);
        }

        classFormat.setFontWeight(QFont::Bold);
        classFormat.setForeground(Qt::magenta);
        rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
        rule.format = classFormat;
        highlightingRules.append(rule);

        //singleLineCommentFormat.setForeground(Qt::red);
        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        //multiLineCommentFormat.setForeground(Qt::red);
        multiLineCommentFormat.setForeground(Qt::darkGreen);

        //quotationFormat.setForeground(Qt::darkGreen);
        quotationFormat.setForeground(Qt::red);
        //rule.pattern = QRegExp("\"(.*)\"");
        rule.pattern = QRegExp("\"([^\"]*)\""); //does not match nested quotes or newlines
        //rule.pattern = QRegExp("((?:[^\\s\"]+)|(?:\"(?:\\\\\"|[^\"])*\"))");
        rule.format = quotationFormat;
        highlightingRules.append(rule);



        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }

    Highlighter::~Highlighter()
    {

    }

    void Highlighter::highlightBlock(const QString &text)
    {
        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
        }
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = commentStartExpression.indexIn(text);

        while (startIndex >= 0) {
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                        + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}
