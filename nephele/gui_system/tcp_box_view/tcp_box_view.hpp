#ifndef TALORION_TCP_BOX_VIEW_HPP
#define TALORION_TCP_BOX_VIEW_HPP

#include <QGridLayout>
#include <QGroupBox>
#include <QMap>
#include <QDockWidget>



namespace talorion {

    class analogView;
    class digital_view;

    class tcp_box_view : public QWidget
    {
        Q_OBJECT
    public:
        explicit tcp_box_view(int entity, QWidget *par = 0);
        ~tcp_box_view();
        Q_DISABLE_COPY(tcp_box_view)

    private:
        void zero_all();
        bool belogs_to_box(int entity);

    private slots:
        void addAIV(int entity);
        void addAOV(int entity);
        void addAV(int entity);
        void addDIV(int entity);
        void addDOV(int entity);
        void addDIOV(int entity);

    private:
        QGridLayout* m_layout;
        //QGridLayout* m_group_box_layout;
        //QGroupBox* m_group_box;

        const int m_entity;

        QMap<int, analogView* > ana_views;
        QMap<int, digital_view* > dig_views;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_VIEW_HPP
