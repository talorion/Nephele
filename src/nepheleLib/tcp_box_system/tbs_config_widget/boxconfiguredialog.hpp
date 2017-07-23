#ifndef BOXCONFIGUREDIALOG_HPP
#define BOXCONFIGUREDIALOG_HPP

#include "abstractmethodwidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QStringListModel>
#include <QDialog>

#include <core/EntityManager.hpp>

namespace talorion {

class BoxConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BoxConfigureDialog(EntityManager::EntityID box, QWidget *par = nullptr,Qt::WindowFlags f = 0);

private:
    Q_DISABLE_COPY(BoxConfigureDialog)

public:
    void setBox(EntityManager::EntityID box);

private:
    void removeAllMethods();

private slots:
    void slot_itemClicked(QListWidgetItem * item);

    void newMethod(EntityManager::EntityID box, EntityManager::EntityID method);

private:
    QLineEdit* serach_field;
    QListWidget* settings_group_list;
    QLabel* settings_widget_label;
    QStringListModel* lst_mdl;
    QGridLayout* mainLayout;
    QMap<QListWidgetItem*,int> list2entity;
    EntityManager::EntityID m_box;

    AbstractMethodWidget* m_MethodWidget;

};

} // namespace talorion

#endif // BOXCONFIGUREDIALOG_HPP
