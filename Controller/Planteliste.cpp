#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QMutex>
#include <vector>
#include "PlanteDatabase.h"
#include "Planteliste.h"


using namespace std;

Planteliste::Planteliste()
{
    openDB();
}

Planteliste::~Planteliste()
{
    closeDB();
}

bool Planteliste::add(PlantValues &pV)
{
    mutex_.lock();
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO plantelist (moisture_set, rotate_set) "
                  "VALUES(:moisture_set, :rotate);");

    query.bindValue(":moisture_set", pV.moisture_set);
    query.bindValue(":rotate", pV.rotate_set);
    if(query.exec())
        qDebug() << "dbAdd OK\n";
    else
    {
        qDebug() << "dbAdd err! \n";
        return false;
    }

   pV.id = query.lastInsertId().toInt();
   mutex_.unlock();
   return true;
}

bool Planteliste::remove(const int &id)
{
    mutex_.lock();
    QSqlQuery query;
    query.prepare("DELETE FROM plantelist WHERE id=:id;");
    query.bindValue(":id", id);

    if(query.exec()){
        qDebug() << "dbUpdate moisture OK\n";
        mutex_.unlock();
        return true;
    }
    else {
        qDebug() << "dbUpdate moisture err! \n";
        mutex_.unlock();
        return false;
    }
}

bool Planteliste::update(PlantValues &PV)
{
    mutex_.lock();
    if(PV.moisture >= 0 && PV.moisture <= 999)
        if(!execUpdate("moisture", PV.moisture, PV.id)) { mutex_.unlock(); return false; }

    if(PV.moisture_set >= 0 && PV.moisture_set <= 999)
       if(!execUpdate("moisture_set", PV.moisture_set, PV.id)) { mutex_.unlock(); return false; }

    if(PV.water >= 0 && PV.water <= 999)
        if(!execUpdate("water", PV.water, PV.id)) { mutex_.unlock(); return false; }

    if(PV.tmp >= 0 && PV.tmp <= 999)
        if(!execUpdate("tmp", PV.tmp, PV.id)) { mutex_.unlock(); return false; }

    if(PV.tmp_set >= 0 && PV.tmp_set <= 999)
        if(!execUpdate("tmp_set", PV.tmp_set, PV.id)) { mutex_.unlock(); return false; }

    if(PV.light >= 0 && PV.light <= 9990)
        if(!execUpdate("light", PV.light, PV.id)) { mutex_.unlock(); return false; }

    if(PV.battery >= 0 && PV.battery <= 999)
        if(!execUpdate("battery", PV.battery, PV.id)) { mutex_.unlock(); return false; }

    if(PV.rotate_set >= 0 && PV.rotate_set <= 999)
        if(!execUpdate("rotate_set", PV.rotate_set, PV.id)) { mutex_.unlock(); return false; }

    if(PV.plantInfo_id >= 0 && PV.plantInfo_id <= 999)
        if(!execUpdate("plantInfo_id", PV.plantInfo_id, PV.id)) { mutex_.unlock(); return false; }

    mutex_.unlock();
    return true;
}

PlantValues Planteliste::get(const int &id)
{
    mutex_.lock();
    QSqlQuery query;
    PlantValues temp;

    query.prepare("SELECT * FROM plantelist WHERE id = :id;");
    query.bindValue(":id", id);

    if(query.exec())
        qDebug() << "dbGet OK\n";
    else{
        qDebug() << "dbGet err! \n";
        temp.id = -1;
        mutex_.unlock();
        return temp;
    }

    if(query.next())
    {
        temp.id = query.value(0).toInt();
        temp.moisture = query.value(1).toInt();
        temp.moisture_set = query.value(2).toInt();
        temp.water = query.value(3).toInt();
        temp.tmp = query.value(4).toInt();
        temp.tmp_set = query.value(5).toInt();
        temp.light = query.value(6).toInt();
        temp.battery = query.value(7).toInt();
        temp.rotate_set = query.value(8).toInt();
        temp.plantInfo_id = query.value(9).toInt();
    }
    else{
        qDebug() << "dbUpdate no rows in DB!\n";
        temp.id = -1;
    }

    mutex_.unlock();
    return temp;
}

vector<PlantValues> Planteliste::getAll()
{
   mutex_.lock();
    vector<PlantValues> temp;
    vector<int> id;
    QSqlQuery query;

    query.prepare("SELECT id FROM plantelist;");

    if(query.exec())
        qDebug() << "dbGet OK\n";
    else
        qDebug() << "dbGet err! \n";

    while(query.next())
        id.push_back(query.value(0).toInt());

    mutex_.unlock();
    while(!id.empty()){
        temp.push_back(get(id.back()));
        id.pop_back();
    }

    return temp;
}

QMutex* Planteliste::getMutex()
{
    return &mutex_;
}

void Planteliste::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Controller.sqlite");

    if (!db.open())
        qDebug() << "Database: Connection failed.";
    else
        qDebug() << "Database: Connection success!";


    setupPL();
    setupPD();

}

void Planteliste::setupPL()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS plantelist("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "moisture INT DEFAULT 0,"
                        "moisture_set INT  DEFAULT 0,"
                        "water INT  DEFAULT 0,"
                        "tmp INT DEFAULT 0,"
                        "tmp_set INT DEFAULT 0,"
                        "light INT DEFAULT 0,"
                        "battery INT DEFAULT 0,"
                        "rotate_set INT DEFAULT 0,"
                        "plantInfo_id INT DEFAULT 0"
                  ");");

    if (!query.exec())
        qDebug() << "setupDB error! - PL" << query.lastError();
    else
        qDebug() << "setupDB done! - PL";
}

void Planteliste::setupPD()
{
    QSqlQuery query;

    query.prepare("CREATE TABLE IF NOT EXISTS planteDatabase("
                        "id INT, "
                        "moisture INT,"
                        "rotate INT,"
                        "tmp INT,"
                        "name TEXT,"
                        "image TEXT"
                  ");");

    if (!query.exec())
        qDebug() << "setupDB error! - PD" << query.lastError();
    else
        qDebug() << "setupDB done! - PD";
}

void Planteliste::closeDB()
{
    db.close();
}

bool Planteliste::execUpdate(QString variable, int val, int id)
{
    QSqlQuery query;
    QString query_string;

    query_string = "UPDATE OR IGNORE plantelist SET " + variable + " = " + QString::number(val) + " WHERE id = " + QString::number(id);
    qDebug() << query_string;
    if(query.exec(query_string)){
        qDebug() << "dbUpdate " << variable << " OK\n";
        return true;
    }
    else{
        qDebug() << "dbUpdate " << variable << " err! \n";
        qDebug() << query.lastError();
        return false;
    }
}






















