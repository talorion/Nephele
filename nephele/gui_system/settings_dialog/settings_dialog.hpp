#ifndef TALORION_SETTINGS_DIALOG_HPP
#define TALORION_SETTINGS_DIALOG_HPP

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>

namespace talorion {

    class settings_dialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit settings_dialog(QWidget *par = 0,Qt::WindowFlags f = 0);
        ~settings_dialog();
        Q_DISABLE_COPY(settings_dialog)
    signals:

    private slots:
        void newSystem(int entity);

    private:
        QLineEdit* serach_field;
        QListView* settings_group_list;
        QLabel* settings_widget_label;
        QWidget* settings_widget;

        QStringListModel* lst_mdl;

        QGridLayout* mainLayout;
    };

} // namespace talorion

#endif // TALORION_SETTINGS_DIALOG_HPP
