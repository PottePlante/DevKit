#include "touchscreen.h"
#include "ui_touchscreen.h"
#include "PlanteDatabase.h"
#include "Planteliste.h"
#include "plantitems_ui.h"
#include "Controlpanel.h"
#include <QProcess>
#include <QDebug>

#include <algorithm>
#include <iterator>

using namespace std;

Touchscreen::Touchscreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Touchscreen)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    QWidget::showFullScreen();

    space_ = new QSpacerItem(120, 50);
    qDebug() << "touchscreen : running";
//    QPushButton* button = new QPushButton;
//    QPushButton* button2 = new QPushButton;
//    ui->gridLayout->addWidget(button);
//    ui->gridLayout->addWidget(button2);
}

Touchscreen::~Touchscreen()
{
    qDebug() << "touchscreen : delete";
    delete ui;
}


void Touchscreen::update(PlanteInfo pI, PlantValues pV)
{
    qDebug() << "touchscreen : update plant id:"<<pV.id;
    if(pV.id == -1)
    {
        qDebug() << "touchscreen : update - error id:"<<pV.id;
        return;
    }

    if(list_.empty())
    {
        qDebug() << "touchscreen : update plant added id:"<<pV.id;
        list_.push_back(new PlantItems_ui(pI, pV));

    }
    else
    {
        for (uint i = 0; i < list_.size(); i++)
        {
                if(list_[i]->update(pI, pV))
                {
                    qDebug() << "touchscreen : update plant - allready exist id:"<<pV.id;
                    return;
                }

        }
        qDebug() << "touchscreen : update plant added id:"<<pV.id;
        list_.push_back(new PlantItems_ui(pI, pV));
    }

    int col = 0;
    int row = 0;
    for(uint i = 0; i < 6; i++)
    {
        if(i < list_.size())
        {
            ui->gridLayout->addWidget((QWidget*)list_[i]->get(), row, col);
        }
        else
        {
            ui->gridLayout->addItem(space_ , row, col);
        }



        if(col == 3)
        {
            col = 0;
            row++;
        }
        else
        {
            col++;
        }

    }
    qDebug() << "touchscreen : gui update";
}

void Touchscreen::init(Controlpanel *ctlPanel, vector<PlanteInfo> pI_vec, vector<PlantValues> pV_vec)
{
    qDebug() << "touchscreen : init";
    ctlPanel_ = ctlPanel;

    ui->plant_combobox->clear();
    ui->type_combobox->clear();
    plant_pos.clear();
    type_pos.clear();
    int size = pV_vec.size();
    qDebug() << "touchscreen : init - update combobox plant";
    for(uint i = 0; i < pV_vec.size(); i++)
    {

        pos_combo pos_tmp;
        pos_tmp.id = pV_vec[i].id;
        pos_tmp.pos = i;
        qDebug() << "touchscreen : init - id"<<pV_vec[i].id << " pos:"<<i;
        plant_pos.push_back(pos_tmp);
        QString tmp = QString("id: ") + QString::number(pV_vec[i].id);
        ui->plant_combobox->addItem(tmp);
        uint i2 = 0;
        for(i2; i2 < pI_vec.size(); i2++)
        {
            int tmp1, tmp2;
            tmp1 = pI_vec[i2].id;
            tmp2 = pV_vec[i].plantInfo_id;
            if(pV_vec[i].plantInfo_id==pI_vec[i2].id)
            {
                update(pI_vec[i2], pV_vec[i]);
                break;
            }
        }

    }

    qDebug() << "touchscreen : init - update combobox plantInfo";
    for(uint i = 0; i < pI_vec.size(); i++)
    {
        pos_combo pos_tmp;
        pos_tmp.id = pV_vec[i].id;
        pos_tmp.pos = i;

        plant_pos.push_back(pos_tmp);
        ui->type_combobox->addItem(pI_vec[i].name);



    }


}


void Touchscreen::on_test_add_clicked()
{
    qDebug() << "touchscreen : test add - click";
    PlanteInfo pI;
    PlantValues pV;

    pI.id = 1;
    pI.moisture = 50;
    pI.name = "test";
    pI.rotate = 10;
    pI.image = "plantimage/test.PNG";

    pV.id = 1;
    pV.plantInfo_id = 1;
    pV.moisture = 50;
    pV.water = 50;

    update(pI, pV);

    pV.id++;
    pV.water = 75;
    update(pI, pV);


}


void Touchscreen::on_get_status_button_clicked()
{
    qDebug() << "touchscreen : status click";
    QString prog = "sh"; //devkit
    QStringList arguments;
    arguments << "-c" <<"ifconfig | grep 'inet ' | awk '{print $2}' | sed 's/addr://'";
    QProcess* process = new QProcess(this);
    process->start(prog , arguments);
    process->waitForFinished();
    QString tmp = process->readAll();
    qDebug() << tmp;
    ui->Status_msg->setPlainText(tmp);
    //ui->label->setText(tmp);

}

void Touchscreen::on_plant_combobox_currentIndexChanged(int index)
{
    qDebug() << "touchscreen : plant combobox index:"<<index;
    int id = -1;
    for(int i = 0; i < plant_pos.size(); i++)
    {
        if(plant_pos[i].pos == index)
        {
            id = plant_pos[i].id;
            break;
        }
    }

    if(id==-1)
    {
        qDebug() << "touchscreen : plant combobox - error index:"<<index;
        return;
    }

    PlantValues pv = ctlPanel_->getPlantValue(id);
    PlanteInfo pi = ctlPanel_->getPlantInfo(pv.plantInfo_id);

    int info_pos = -1;

    for(int i = 0; i < type_pos.size(); i++)
    {
        if(pi.id==type_pos[i].id)
        {
            qDebug() << "touchscreen : plant combobox - update index:"<<index;
            ui->type_combobox->setCurrentIndex(type_pos[i].pos);
            break;
        }
    }


}
