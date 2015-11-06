#include "Controlpanel.h"
#include "touchscreen.h"
#include "WiFi.h"

using namespace std;


Controlpanel::Controlpanel(PlanteDatabase *database, Planteliste *list, Touchscreen *ui, WiFi *wifi, bool test)
    :database_(database), list_(list), ui_(ui), wifi_(wifi)
{

    if(test)
    {
        qDebug() << "Contolpanel : run test";
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
            pV_vec.push_back(pv_tmp);
        }
        ui_->init(this, pI_vec, pV_vec);

    }
    else
    {
        ui_->init(this, database_->getAll(), list_->getAll());
    }

    //sæt wifi op
    wifi_->init(this);


    qRegisterMetaType<PlantValues>("PlantValues");
    qRegisterMetaType<PlanteInfo>("PlanteInfo");


    QThread *workerThread = new QThread;

    wifi_->moveToThread(workerThread);
    connect(workerThread, SIGNAL(started()), wifi_, SLOT(run()));
    connect(this, SIGNAL(guiUpdate(PlanteInfo, PlantValues)), ui_, SLOT(update(PlanteInfo, PlantValues)));
    workerThread->start();

    ui_->show();
    qDebug() << "Contolpanel : init complete";
}


void Controlpanel::removePlant(int id)
{
    qDebug() << "Contolpanel : remove plant id:"<<id;
    list_->remove(id);
}

void Controlpanel::changePlant(PlantValues pV)
{
    qDebug() << "Contolpanel - !not build! : change plant id:" << pV.id;
}

const std::vector<PlanteInfo> Controlpanel::getPlantInfo() const
{
    qDebug() << "Contolpanel : getPlantInfo vector";
    return database_->getAll();
}

PlantValues Controlpanel::getPlantValue(int id)
{
    qDebug() << "Contolpanel : getPlantValue id:" << id;
    return list_->get(id);
}


PlanteInfo Controlpanel::getPlantInfo(int id)
{
    qDebug() << "Contolpanel : getPlantInfo id:"<<id;
    return database_->get(id);
}




void Controlpanel::updatePlantValue(PlantValues pV)
{
    qDebug() << "Contolpanel : update plant id:"<<pV.id;
    PlantValues newpV = pV;


    if(list_->get(pV.id).id == -1)
    {
        list_->add(newpV);
        newpV.plantInfo_id = 1;
        list_->update(newpV);
        qDebug() << "Contolpanel : new plant added id:"<<newpV.id;
    }
    else
    {

        PlantValues tmp = list_->get(pV.id);

        newpV.moisture_set = tmp.moisture_set;
        newpV.tmp_set = tmp.tmp_set;
        if(!list_->update(newpV))
        {
            qDebug() << "Contolpanel : error plant not updated. plant id:"<<newpV.id;
            return;
        }

    }
     qDebug() << "Contolpanel : wifi update plant id:"<<newpV.id;
    wifi_->update(newpV);
     qDebug() << "Contolpanel : gui update plant id:"<<newpV.id;


    emit guiUpdate(database_->get(newpV.plantInfo_id), newpV);
    //ui_->update(database_->get(newpV.plantInfo_id), newpV);

    qDebug() << "Contolpanel : update done id:"<<newpV.id;
    return;
}
