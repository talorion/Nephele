#ifndef TALORION_TCP_BOX_VIEW_HPP
#define TALORION_TCP_BOX_VIEW_HPP

#include <QGridLayout>
#include <QGroupBox>
#include <QMap>

#include "core/AbstractSystemView.hpp"


namespace talorion {

    class analogView;
    class digital_view;

    //class tcp_box_view : public QWidget
    class tcp_box_view : public AbstractSystemView
    {
        Q_OBJECT
    public:
        explicit tcp_box_view(int entity, QWidget *par = 0);
        ~tcp_box_view();
        Q_DISABLE_COPY(tcp_box_view)

        int entity() const;
        int numberOfValueViews()const;

        void setLabelMinimumWidth(int labelMinimumWidth);

    signals:
        void labelMinimumWidthChanged(int labelMinimumWidth);

    private:
        void zero_all();
        bool belogs_to_box(int entity);
        void appendWidget(QWidget* wdg);

    private slots:
        void disconnect_tcp_box(int entity);

        void addAIV(int entity);
        void addAOV(int entity);
        void addAV(int entity);
        void addDIV(int entity);
        void addDOV(int entity);
        void addDIOV(int entity);

        void removeAIV(int entity);
        void removeAOV(int entity);
        void removeAV(int entity);
        void removeDIV(int entity);
        void removeDOV(int entity);
        void removeDIOV(int entity);

    private:
        QGridLayout* m_layout;
        //QGridLayout* m_group_box_layout;
        //QGroupBox* m_group_box;

        const int m_entity;

        QMap<int, analogView* > ana_views;
        QMap<int, digital_view* > dig_views;

        int m_labelMinimumWidth;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_VIEW_HPP
