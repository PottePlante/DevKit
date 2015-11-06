#include "PlanteDatabase.h"

using namespace std;

PlanteDatabase::PlanteDatabase(QMutex* MX)
{
    //openDB();
    mutex_=MX;
}

PlanteDatabase::~PlanteDatabase()
{
    //closeDB();
}

PlanteInfo PlanteDatabase::get(const int &id)
{

    mutex_->lock();
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
        temp.tmp = query.value(3).toInt();
        temp.name = query.value(4).toString();
        temp.image = query.value(5).toString();
    }
    else{
        qDebug() << "get no rows in DB!\n";
        temp.id = -1;
    }

    mutex_->unlock();
    return temp;
}

vector<PlanteInfo> PlanteDatabase::getAll()
{
    mutex_->lock();
    vector<PlanteInfo> temp;
    vector<int> id;
    QSqlQuery query;

    query.prepare("SELECT id FROM planteDatabase;");

    if(query.exec())
        qDebug() << "dbGet OK\n";
    else
        qDebug() << "dbGet err! \n";

    while(query.next())
        id.push_back(query.value(0).toInt());

    mutex_->unlock();
    while(!id.empty()){
        temp.push_back(get(id.back()));
        id.pop_back();
    }
    return temp;
}

/*void PlanteDatabase::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "PD");
    db.setDatabaseName("./planteDatabase.sqlite");

    if (!db.open())
        qDebug() << "Database: Connection failed. - PD";
    else
        qDebug() << "Database: Connection success! - PD";

    setupDB();
}*/

/*void PlanteDatabase::setupDB()
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
        qDebug() << "setupDB error! - PD";
    else
        qDebug() << "setupDB done! - PD";
}*/


/*void PlanteDatabase::closeDB()
{
    db.close();
}*/
