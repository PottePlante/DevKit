#include "Controlpanel.h"
#include "PlanteDatabase.h"
#include "Planteliste.h"
#include "touchscreen.h"
#include "WiFi.h"
#include <algorithm>
#include <iterator>

using namespace std;



Controlpanel::Controlpanel(PlanteDatabase *database, Planteliste *list, Touchscreen *ui, WiFi *wifi, bool test)
    :database_(database), list_(list), ui_(ui), wifi_(wifi)
{
    if(test)
    {
        vector<PlantValues> pV_vec;
        vector<PlanteInfo> pI_vec;
        PlanteInfo pi_tmp_1 =
        {
            .id = 0,//-1 for intet valgt id
            .moisture = 50,
            .rotate = 10,
            .name = "blomst",
            .image = "plantimage/test.PNG"
        };

        pI_vec.push_back(pi_tmp_1);
        pi_tmp_1.id = 1;
        pi_tmp_1.name = "solsik";

        pI_vec.push_back(pi_tmp_1);

        for(int i = 0; i < 4; i++)
        {
            PlantValues pv_tmp = {
                .id = i,
                .moisture = rand()%100,
                .moisture_set = rand()%100,
                .water = rand()%100,
                .tmp = rand()%100,
                .tmp_set = rand()%100,
                .light = rand()%100,
                .battery = rand()%100,
                .rotate_set = rand()%100,
                .plantInfo_id = rand()%2
            };
            pv_tmp.id;
            pV_vec.push_back(pv_tmp);
        }
        ui_->init(this, pI_vec, pV_vec);

    }
    else
    {
        ui_->init(this, database_->getAll(), list_->getAll());
    }

    wifi_->init(this);
    ui_->show();

}

Controlpanel::~Controlpanel()
{}



void Controlpanel::removePlant(int id)
{
    list_->remove(id);
}

void Controlpanel::changePlant(PlantValues pV)
{

}

const std::vector<PlanteInfo> Controlpanel::getPlantInfo() const
{
    return database_->getAll();
}

PlantValues Controlpanel::getPlantValue(int id)
{
    return list_->get(id);
}


PlanteInfo Controlpanel::getPlantInfo(int id)
{
    return database_->get(id);
}




bool Controlpanel::updatePlantValue(PlantValues pV)
{

    PlantValues newpV = pV;
    if(pV.id == -1)
    {
        list_->add(newpV);
    }
    else
    {
        PlantValues tmp = list_->get(pV.id);

        newpV.moisture_set = tmp.moisture_set;
        newpV.tmp_set = tmp.tmp_set;
        if(!list_->update(newpV))
            return false;

    }

    wifi_->update(newpV);

    ui_->update(database_->get(newpV.plantInfo_id), newpV);
    return true;
}
