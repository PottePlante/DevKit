#include "PlanteDatabase.h"

using namespace std;

PlanteDatabase::PlanteDatabase()
{
    openDB();
}

PlanteDatabase::~PlanteDatabase()
{
    closeDB();
}

PlanteInfo PlanteDatabase::get(const int &id)
{
    mutex_.lock();
    QSqlQuery query;
    PlanteInfo temp;

    query.prepare("SELECT * FROM planteDatabase WHERE id = :id;");
    query.bindValue(":id", id);

    if(query.exec())
        qDebug() << "dbGet OK\n";
    else{
        qDebug() << "dbGet err! \n";
        temp.id = -1;
    }

    if(query.next())
    {
        temp.id = query.value(0).toInt();
        temp.moisture = query.value(1).toInt();
        temp.rotate = query.value(2).toInt();
        temp.name = query.value(3).toString();
        temp.image = query.value(4).toString();
    }
    else{
        qDebug() << "get no rows in DB!\n";
        temp.id = -1;
    }

    mutex_.unlock();
    return temp;
}

vector<PlanteInfo> PlanteDatabase::getAll()
{
    mutex_.lock();
    vector<PlanteInfo> temp;
    QSqlQuery query;

    query.prepare("SELECT id FROM plantelist;");

    if(query.exec())
        qDebug() << "dbGet OK\n";
    else
        qDebug() << "dbGet err! \n";

    while(query.next())
        temp.push_back(get(query.value(0).toInt()));

    mutex_.unlock();
    return temp;
}

void PlanteDatabase::openDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./planteDatabase.sqlite");

    if (!db.open())
        qDebug() << "Database: Connection failed.\n";
    else
        qDebug() << "Database: Connection success!\n";

    setupDB();
}

void PlanteDatabase::setupDB()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS planteDatabase("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "moisture INT,"
                        "rotate INT,"
                        "name TEXT,"
                        "image TEXT"
                  ");");

    if (!query.exec())
        qDebug() << "setupDB error!\n";
    else
        qDebug() << "setupDB done!\n";
}


void PlanteDatabase::closeDB()
{
    QSqlDatabase db;
    db.close();
}
