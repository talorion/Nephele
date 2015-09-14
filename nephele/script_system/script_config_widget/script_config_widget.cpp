#include "script_config_widget.hpp"

#include <QScriptEngine>
#include <QScriptValueIterator>

#include "core/entity_manager.hpp"

namespace talorion {

    script_config_widget::script_config_widget(QWidget *par) :
        abstract_configuration_widget(par),
        mainLayout(NULL),
        script_object_tree(NULL),
        my_engine(-1),
        treeitems()
    {

        script_object_tree = new QTreeWidget();

        mainLayout = new QGridLayout();
        mainLayout->addWidget(script_object_tree,0,0,1,1);

        setLayout(mainLayout);


    }

    script_config_widget::~script_config_widget()
    {
        delete script_object_tree;
        delete mainLayout;

        foreach (QTreeWidgetItem* var, treeitems) {
            if(var)
                delete var;
        }
    }

    void script_config_widget::update_script_tree(){
        //        script_object_tree->setColumnCount(1);
        //        QList<QTreeWidgetItem *> items;
        //        for (int i = 0; i < 10; ++i)
        //            items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
        //        script_object_tree->insertTopLevelItems(0, items);

        if(my_engine >= 0){
            QScriptEngine* eng =  entity_manager::get_instance()->get_qt_script_engine_component(my_engine);

            if(eng == NULL)
                return;

            QScriptValue object =eng->globalObject();
            QList<QTreeWidgetItem *> items = rec_get_values(object);
            script_object_tree->insertTopLevelItems(0, items);
        }
    }

    QList<QTreeWidgetItem *> script_config_widget::rec_get_values(QScriptValue object)
    {
        QList<QTreeWidgetItem *> items;

        QScriptValueIterator it(object);
        while (it.hasNext()) {
            it.next();
            //qDebug() << it.name() << ": " << it.value().toString();
            QScriptValue v = it.value();
            QTreeWidgetItem* qtwi = new QTreeWidgetItem((QTreeWidget*)0, QStringList(it.name()));
            treeitems.append(qtwi);

            qtwi->setIcon(0,QIcon(":/images/images/new.png"));

            if(v.isFunction())
                qtwi->setIcon(0,QIcon(":/images/images/func.png"));

            if(v.isQObject()){
                qtwi->setIcon(0,QIcon(":/images/images/class.png"));
                qtwi->addChildren(rec_get_values(v));
            }

            //if(v.isObject()){
            //    qtwi->setIcon(0,QIcon(":/images/images/class.png"));
            //qtwi->addChildren(rec_get_values(v));
            //}

            if(v.isArray() || v.isBool() || v.isBoolean() || v.isDate() || v.isNumber() || v.isString() || v.isVariant()){
                qtwi->setIcon(0,QIcon(":/images/images/var.png"));
                qtwi->setText(1, v.toString());
            }


            items.append(qtwi);
        }
        return items;
    }

    void script_config_widget::init_engine(int entity)
    {
        if(entity<0)
            return;

        //        QString tmp= entity_manager::get_instance()->get_script_file_component(entity);
        //        if(!tmp.isEmpty())
        //            loadFile(tmp);

        my_engine = entity;
    }

    void script_config_widget::do_refresh_data()
    {
        QList<int> script_engines = entity_manager::get_instance()->get_all_Qt_Script_Engines();
        if(!script_engines.isEmpty())
            init_engine(script_engines[0]);

        update_script_tree();
    }



} // namespace talorion

