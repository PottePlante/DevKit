#ifndef PLANTELISTE_H
#define PLANTELISTE_H
#include <vector>
#include <QMutex>
#include <QSqlDatabase>

struct PlanteInfo;

struct PlantValues
{
    int id;
    int moisture;
    int moisture_set;
    int water;
    int tmp;
    int tmp_set;
    int light;
    int battery;
    int rotate_set;
    int plantInfo_id;
};

class Planteliste
{
public:
    Planteliste();
    ~Planteliste();

    //bool set(const PlantValues &pV);                  // det samme som update men ændre kun på de værdier der har set på sig.
    bool add(PlantValues &pV);                          // Tilføje en ny plante, hvis id'et enten er -1 eller et id der ikke findes. Det id den får tilgivet, bliver returneret igennem &pV!(altså pV.id = #nytID)
    bool remove(const int &id);                         // fjerner en plante med et given id, returnere sand hvis planten blev fjernet returnere falsk hvis ikke.
    bool update(PlantValues &pV);                       // opdatere en bestemt plante, hvis fejl returner falsk ellers returner sand.
    PlantValues get(const int &id);               // returner en plante udfra et id.
    std::vector<PlantValues> getAll();            // returner alle planter der er i databasen i en vector.
    QMutex* getMutex();
    bool xmlUpdate();

private:
    void openDB();
    void setupPL();
    void setupPD();
    void closeDB();
    bool execUpdate(QString, int, int);
    QSqlDatabase db;
    QMutex mutex_;
};

#endif // PLANTELISTE_H
