#ifndef TALORION_SETTINGS_DIALOG_HPP
#define TALORION_SETTINGS_DIALOG_HPP

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QStringListModel>

#include <gui_system/CustomWidgets/TitleLabel.hpp>

namespace talorion {

    class abstract_configuration_widget;

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
        void slot_itemClicked(QListWidgetItem * item);

    private:
        QLineEdit* serach_field;
        QListWidget* settings_group_list;
        TitleLabel* settings_widget_label;
        abstract_configuration_widget* settings_widget;

        QStringListModel* lst_mdl;

        QGridLayout* mainLayout;

        QMap<QListWidgetItem*,int> list2entity;


    };

} // namespace talorion

#endif // TALORION_SETTINGS_DIALOG_HPP
