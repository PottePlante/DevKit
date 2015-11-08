#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <vector>
#include <QObject>
#include <QThread>
#include <algorithm>
#include <iterator>
#include <QDebug>
#include "PlanteDatabase.h"
#include "Planteliste.h"


class WiFi;
class Touchscreen;

class Controlpanel : public QObject
{
    Q_OBJECT
public:
    Controlpanel(PlanteDatabase*, Planteliste*, Touchscreen*, WiFi* ,bool test = false);

    bool updataPlanteInfo(PlantValues);
    void removePlant(int id);
    void changePlant(PlantValues);
    const std::vector<PlanteInfo> getPlantInfo() const;
    const std::vector<PlantValues> getPlantValue() const;
    PlantValues getPlantValue(int id);
    PlanteInfo getPlantInfo(int id);
    void updatePlantValue(PlantValues);
signals:
    void guiUpdate(PlanteInfo, PlantValues);
    void guiRemovePlant(int);
private:

    PlanteDatabase* database_;
    Planteliste* list_;
    Touchscreen* ui_;
    WiFi* wifi_;
};
#endif // CONTROLPANEL_H
