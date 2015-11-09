#ifndef SCRIPT_DIALOGES_HANDLER_H
#define SCRIPT_DIALOGES_HANDLER_H

#include "abstract_scriptable_object.hpp"

namespace talorion {

    class script_dialoges_handler : public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        explicit script_dialoges_handler(QObject *par = 0);
        ~script_dialoges_handler();
        Q_DISABLE_COPY(script_dialoges_handler)

    public:
        Q_INVOKABLE double showNumericDialog();
        Q_INVOKABLE QString showStringDialog();
        Q_INVOKABLE QString showFileDialog();
        Q_INVOKABLE void showInfoDialog(QString msg);
        Q_INVOKABLE void showPlotDialog();

    signals:
        void open_numeric_dialog();
        void open_string_dialog();
        void open_file_dialog();
        void open_info_dialog(const QString &msg);
        void open_plot_dialog();

        void dialog_finished();

    private slots:    
        void slot_dialog_finished(double val);
        void slot_dialog_finished(QString val);

    private:
        double m_tmp_num;
        QString m_tmp_str;

    };
}

#endif // SCRIPT_DIALOGES_HANDLER_H
