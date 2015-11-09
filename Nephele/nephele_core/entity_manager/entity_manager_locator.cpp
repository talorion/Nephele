#include "entity_manager_locator.hpp"

namespace talorion {

    null_entity_manager entity_manager_locator::null_manager_;
    abstract_entity_manager* entity_manager_locator::manager_ = NULL;
    //abstract_entity_manager* entity_manager_locator::manager_ = &entity_manager_locator::null_manager_;

    entity_manager_locator::entity_manager_locator(QObject *par) :
        QObject(par)
    {

    }

    entity_manager_locator::~entity_manager_locator()
    {

    }

    void entity_manager_locator::provide(abstract_entity_manager *entity_manager){
        if(entity_manager == NULL){
            //Revert to null service
            manager_ = &null_manager_;
        }else{
            manager_ = entity_manager;
        }
    }

} // namespace talorion

