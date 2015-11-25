#include "PlanteDatabase.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDomDocument>
#include <QDomNode>

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

bool PlanteDatabase::xmlUpdate()
{
    QSqlQuery query;
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl("http://plante.nytsite.dk")));
    QEventLoop event;
    QEventLoop::connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    QDomDocument doc;
    doc.setContent(response->readAll());

    //Tjekker http status kode
    if(response->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        qDebug() << "http status fejl: " << response->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        return 0;
    }

    QDomNodeList list = doc.lastChild().childNodes();

    mutex_->lock();
    //Tømmer DB
    query.prepare("DELETE FROM planteDatabase;");

    if (!query.exec()){
        qDebug() << "Delete error! - PD" << query.lastError();
        mutex_->unlock();
        return false;
    }
    else
        qDebug() << "Delete done! - PD";


    //Updatere DB
    for (int i = 0; i < list.count(); i++) {
        query.clear();
        QDomElement node = list.at(i).toElement();

        query.prepare("INSERT OR IGNORE INTO planteDatabase (id, moisture, rotate, tmp, name, image) "
                      "VALUES(:id, :moisture, :rotate, :tmp, :name, :image);");

        query.bindValue(":id", node.attribute("id"));
        query.bindValue(":moisture", node.attribute("moisture"));
        query.bindValue(":rotate", node.attribute("rotate"));
        query.bindValue(":tmp", node.attribute("tmp"));
        query.bindValue(":name", node.attribute("name"));
        query.bindValue(":image", node.attribute("image"));

        if(query.exec())
            qDebug() << "PD update from web OK";
        else {
            qDebug() << "PD update from web err!";
            qDebug() << query.lastError();
            mutex_->unlock();
            return false;
        }
    }

    mutex_->unlock();
    return true;
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
