#ifndef DATAAQUISITIONCONFIGURATIONWIDGET_HPP
#define DATAAQUISITIONCONFIGURATIONWIDGET_HPP

#include "core/abstract_configuration_widget.hpp"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "../DaqDllEntity/DaqDllEntity.hpp"


namespace talorion {

class DataAquisitionConfigurationWidget : public abstract_configuration_widget
{
    Q_OBJECT
public:
    explicit DataAquisitionConfigurationWidget(QWidget *par = 0);
    virtual ~DataAquisitionConfigurationWidget()=default;

private:
    Q_DISABLE_COPY(DataAquisitionConfigurationWidget)

public:
    QSpinBox *userDataUpdaterateField() const;
    QLineEdit *userDataPathField() const;
    QSpinBox *timeoutField() const;
    QLineEdit *dllNameField() const;

private slots:
    void browseButtonPressed();
    void timoutValueChanged(int val);
    void userDataUpdaterateChanged(int val);
    void userDataPathChanged(QString val);
    void userDataPathEditFinished();

private:
    QGridLayout* m_mainLayout;
    QLabel* m_name_label;
    QLabel* m_timeout_label;
    QLineEdit* m_dll_name_field;
    QLabel* m_user_data_updaterate_label;
    QSpinBox* m_user_data_updaterate_field;
    QSpinBox* m_timeout_field;
    QPushButton* m_browse_button;
    QLabel* m_user_data_path_label;
    QLineEdit* m_user_data_path_field;
    QSpacerItem* m_horizontal_spacer;

    EntityManager::EntityID m_current_entity;
    DaqDllEntity* m_daqDllEnitity;


    // AbstractConfigurationWidget interface
private:
    virtual void do_refresh_data() Q_DECL_OVERRIDE;
};

} // namespace talorion

#endif // DATAAQUISITIONCONFIGURATIONWIDGET_HPP
