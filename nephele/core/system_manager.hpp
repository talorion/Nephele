#ifndef TALORION_SYSTEM_MANAGER_HPP
#define TALORION_SYSTEM_MANAGER_HPP

#include <QMutex>
#include <QObject>
#include <QtCore>

#include "abstract_system.hpp"

namespace talorion {

    class system_manager : public QObject
    {
        Q_OBJECT
    private:
        explicit system_manager(QObject *par = 0);
        ~system_manager();
        Q_DISABLE_COPY(system_manager)

    public:
        static system_manager *get_instance();
        static void destroy();

        template<typename T>
        void register_new_system();

        QList<int> get_all_systems()const;

   private:
        void load_system(abstract_system* sys);

    private:
        QSet<abstract_system*> m_systems;

    private:
        static system_manager* _instance;
        static QMutex _mutex;
    };

    template<typename T>
    void system_manager::register_new_system()
    {
        static_assert(std::is_base_of<abstract_system,T>::value,"T must be derived from abstract_system!");
        //std::is_base_of<base, derived>::value;
        T* tmp = new T();
        load_system(tmp);
    }

//    template <typename T>
//    void system_manager::register_new_system<T*>() {
//        static_assert(false, "T must not be a pointer type");
//    }

} // namespace talorion

#endif // TALORION_SYSTEM_MANAGER_HPP
