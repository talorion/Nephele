#include "config_file.hpp"


#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/EntityManager.hpp"
#include "core/EventManager.hpp"

namespace talorion {

    config_file::config_file(QObject *par):
        abstract_scriptable_object("cfg_file", par)
    {
        connect(this,SIGNAL(change_set_value(int,double)),EventManager::get_instance(),SIGNAL(change_analogSet_component(int,double)));
    }

    config_file::~config_file()
    {

    }

    void config_file::loadFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly)) {
            //            QMessageBox::warning(this, tr("Application"),
            //                                 tr("Cannot read file %1:\n%2.")
            //                                 .arg(fileName)
            //                                 .arg(file.errorString()));
            return;
        }

        //QTextStream in(&file);
        //#ifndef QT_NO_CURSOR
        //        QApplication::setOverrideCursor(Qt::WaitCursor);
        //#endif
        //editor->setPlainText(in.readAll());
        //========
        //if (!file.open(QIODevice::ReadOnly)) {
        //    QMessageBox::warning(this, tr("Application"),tr("Couldn't open save file."));
        //    return ;
        //}
        QByteArray saveData = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        read(loadDoc.object());
        //========
        //#ifndef QT_NO_CURSOR
        //        QApplication::restoreOverrideCursor();
        //#endif

        //        setCurrentFile(fileName);
        //        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    bool config_file::saveFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly)) {
            //            QMessageBox::warning(this, tr("Application"),
            //                                 tr("Cannot write file %1:\n%2.")
            //                                 .arg(fileName)
            //                                 .arg(file.errorString()));
            return false;
        }

        //QTextStream out(&file);
        //#ifndef QT_NO_CURSOR
        //        QApplication::setOverrideCursor(Qt::WaitCursor);
        //#endif
        //out << editor->toPlainText();
        //=========
        //if (!file.open(QIODevice::WriteOnly)) {
        //    QMessageBox::warning(this, tr("Application"), tr("Couldn't open save file."));
        //    return false;
        //}

        QJsonObject gameObject;
        write(gameObject);
        QJsonDocument saveDoc(gameObject);
        file.write(saveDoc.toJson());
        //=========
        //#ifndef QT_NO_CURSOR
        //        QApplication::restoreOverrideCursor();
        //#endif

        //        setCurrentFile(fileName);
        //        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }

    void config_file::read(const QJsonObject &json)
    {
        bool ok;
        //int entity;
        int box_id;
        int bid;
        double val;
        QJsonObject obj;

        foreach(QString box_nme, json.keys()){

            if(json[box_nme].isObject()){
                obj = json[box_nme].toObject();
                foreach (QString nme, obj.keys()) {

                    QList<int> entities =  EntityManager::get_instance()->get_entity_by_name(nme);

                    foreach (int entity, entities) {
                        box_id = EntityManager::get_instance()->get_box_id_component(entity);
                        bid=  box_nme.toInt(&ok);

                        if(ok && bid == box_id){
                            val =  obj.value(nme).toDouble(0);
                            emit change_set_value(entity,val);
                        }
                    }
                }
            }
        }
    }

    void config_file::write(QJsonObject &json) const
    {
        int box_id;
        QString box_nme;
        //QJsonObject obj;
        QString nme;
        double val;

        QJsonObject::iterator it;
        //foreach (int entity , entity_manager::get_instance()->get_all_AnalogValues()) {
        foreach (int entity , EntityManager::get_instance()->get_all_Values()) {

            box_id = EntityManager::get_instance()->get_box_id_component(entity);
            box_nme= QString::number(box_id);

            QJsonObject obj;
            it = json.find(box_nme);
            if(it == json.end())
                it= json.insert(box_nme, obj);

            if((*it).isObject())
                obj = (*it).toObject();

            nme= EntityManager::get_instance()->get_name_component(entity);
            val = EntityManager::get_instance()->get_analogSetValue_component(entity);

            if(!nme.isEmpty()){
                obj.insert(nme, val);
            }

            json.insert(box_nme, obj);
        }
    }

} // namespace talorion

