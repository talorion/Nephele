#ifndef TALORION_ENTITY_MANAGER_LOCATOR_HPP
#define TALORION_ENTITY_MANAGER_LOCATOR_HPP

#include <QObject>

#include "abstract_entity_manager.hpp"
#include "null_entity_manager.hpp"

namespace talorion {

    class entity_manager_locator : public QObject
    {
        Q_OBJECT
    private:
        explicit entity_manager_locator(QObject *par = 0);
        ~entity_manager_locator();
        Q_DISABLE_COPY(entity_manager_locator)
    public:

        static abstract_entity_manager& manager(){return *manager_;}
        static abstract_entity_manager* get_instance(){return manager_;}

        static void provide(abstract_entity_manager* entity_manager);

    private:
        static abstract_entity_manager* manager_;
        static null_entity_manager null_manager_;

    };

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_LOCATOR_HPP
