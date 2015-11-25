#ifndef PLANTEDATABASE_H
#define PLANTEDATABASE_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QListWidget>
#include <QString>
#include <QVariant>
#include <vector>
#include <QMutex>

using namespace std;

struct PlanteInfo
{
    int id;//-1 for intet valgt id
    int moisture;
    int rotate;
    int tmp;
    QString name;
    QString image;
};

class PlanteDatabase
{
public:
    PlanteDatabase(QMutex*);
    ~PlanteDatabase();

    PlanteInfo get(const int &id);            // returner planteInformation udfra et id. hvis fejl returner "planteinfo = {id = -1};"
    std::vector<PlanteInfo> getAll();   // returner alle planteInformationer i en vector.
    bool xmlUpdate();
private:
    //void openDB();
    //void setupDB();
    //void closeDB();
    QMutex* mutex_;
};

#endif // PLANTEDATABASE_H
