#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <vector>

//prototyper
struct PlantValues;
struct PlanteInfo;

class PlanteDatabase;
class Planteliste;
class Touchscreen;
class WiFi;

//


class Controlpanel
{
public:
    Controlpanel(PlanteDatabase*, Planteliste*, Touchscreen*, WiFi*, bool test = false);
    ~Controlpanel();

    bool updataPlanteInfo(PlantValues);
    void removePlant(int id);
    void changePlant(PlantValues);
    const std::vector<PlanteInfo> getPlantInfo() const;
    bool updatePlantValue(PlantValues);

private:

    PlanteDatabase* database_;
    Planteliste* list_;
    Touchscreen* ui_;
    WiFi* wifi_;
};
#endif // CONTROLPANEL_H
