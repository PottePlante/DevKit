#include "touchscreen.h"
#include "ui_touchscreen.h"

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
    //setWindowFlags(Qt::CustomizeWindowHint);
    //QWidget::showFullScreen();

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
    for(int i = 0; i < list_.size(); i++)
        delete list_[i];

    delete space_;
    delete ui;
}

void Touchscreen::init(Controlpanel *ctlPanel, vector<PlanteInfo> pI_vec, vector<PlantValues> pV_vec)
{
    qDebug() << "touchscreen : init pI size " << pI_vec.size() << " pV size " << pV_vec.size();
    ctlPanel_ = ctlPanel;

    ui->plant_combobox->clear();
    ui->type_combobox->clear();
    plant_pos.clear();
    type_pos.clear();
    qDebug() << "touchscreen : init - update combobox plant";
    for(int i = 0; i < pV_vec.size(); i++)
    {

        pos_combo pos_tmp;
        pos_tmp.id = pV_vec[i].id;
        pos_tmp.pos = i;
        qDebug() << "touchscreen : init - id"<<pV_vec[i].id << " pos:"<<i;
       // plant_pos.push_back(pos_tmp);
//        QString tmp = QString("id: ") + QString::number(pV_vec[i].id);
//        ui->plant_combobox->addItem(tmp);

        for(uint i2 = 0; i2 < pI_vec.size(); i2++)
        {

            if(pV_vec[i].plantInfo_id==pI_vec[i2].id)
            {
                update(pI_vec[i2], pV_vec[i]);
                break;
            }
        }

    }

    qDebug() << "touchscreen : init - update combobox plantInfo";
    pos_combo pos_tmp ={
        //.id = pI_vec[i].id,
        .id = 1,
        .pos = 1
    };

    for(int i = 0; i < pI_vec.size(); i++)
    {
        qDebug() << "touchscreen : added to combobox planttype id " << pI_vec[i].id;
        pos_combo pos_tmp ={
          .id = pI_vec[i].id,
          .pos = i
         };
        type_pos.push_back(pos_tmp);
    }

    updateCombobox();
    updateParameter();
        qDebug() << "touchscreen : init - update done";

}

//-------private slots metoder

void Touchscreen::removePlant(int id)
{
    vector<pos_combo> plant_pos_tmp = plant_pos;
    plant_pos.clear();
    for(int i = 0; i < plant_pos_tmp.size(); i++)
    {
        if(plant_pos_tmp[i].id != id)
            plant_pos.push_back(plant_pos_tmp[i]);
    }

    vector<PlantItems_ui*> list_tmp = list_;
    list_.clear();

    for(int i = 0; i < list_tmp.size(); i++)
    {
        if(list_tmp[i]->getId() != id)
            list_.push_back(list_tmp[i]);
        else
            delete list_tmp[i];
    }

    qDebug() << "removed plant id: " << id << " size : " <<list_.size();

    updateCombobox();
    updateParameter();
    updateStatusPage();

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

    pos_combo tmp = {
        .id = pV.id,
        .pos = plant_pos.size()
    };

    plant_pos.push_back(tmp);

    updateCombobox();
    updateParameter();
    qDebug() << "touchscreen : update plant to combox id:"<<pV.id;

    updateStatusPage();
    qDebug() << "touchscreen : gui update";
}

//-------private metoder

void Touchscreen::updateCombobox()
{
    ui->plant_combobox->clear();
    ui->type_combobox->clear();

    for(uint i = 0; i < plant_pos.size(); i++)
    {
        PlantValues tmp_val = ctlPanel_->getPlantValue(plant_pos[i].id);
        QString tmp = QString("id : ") + QString::number(tmp_val.id);
        ui->plant_combobox->addItem(tmp);
        plant_pos[i].pos = i;
    }


    for(uint i = 0; i < type_pos.size(); i++)
    {
        PlanteInfo tmp_info = ctlPanel_->getPlantInfo(type_pos[i].id);
        ui->type_combobox->addItem(tmp_info.name);
        type_pos[i].pos = i;
    }

}

void Touchscreen::updateStatusPage()
{
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
}

void Touchscreen::updateParameter()
{
    PlantValues tmp_val = ctlPanel_->getPlantValue(plant_pos[ui->plant_combobox->currentIndex()].id);
    PlanteInfo tmp_info = ctlPanel_->getPlantInfo(tmp_val.plantInfo_id);

    for(uint i = 0; i < type_pos.size(); i++)
    {
        if(tmp_info.id == type_pos[i].id)
        {
            ui->type_combobox->setCurrentIndex(i);
        }
    }

    updateParameter_slider(tmp_val);
}

void Touchscreen::updateParameter_slider(PlanteInfo tmp)
{
    ui->moisture_slider->setValue(tmp.moisture);
    ui->rotate_slider->setValue(tmp.rotate);
    ui->temperature_slider->setValue(tmp.tmp);

    ui->moisture_label->setText(QString("Moisture ") + QString::number(tmp.moisture) + "%");
    ui->temperature_label->setText(QString("Max temperature ") + QString::number(tmp.tmp)+ "C");
    ui->rotate_label->setText(QString("Rotate ") + QString::number(tmp.rotate) + " deg/hour");
}

void Touchscreen::updateParameter_slider(PlantValues tmp)
{
    ui->moisture_slider->setValue(tmp.moisture_set);
    ui->rotate_slider->setValue(tmp.rotate_set);
    ui->temperature_slider->setValue(tmp.tmp_set);

    ui->moisture_label->setText(QString("Moisture ") + QString::number(tmp.moisture_set) + "%");
    ui->temperature_label->setText(QString("Max temperature ") + QString::number(tmp.tmp_set)+ "C");
    ui->rotate_label->setText(QString("Rotate ") + QString::number(tmp.rotate_set) + " deg/hour");
}

//-------gui

void Touchscreen::on_Change_button_clicked()
{
    PlantValues tmp;

    tmp.id = plant_pos[ui->plant_combobox->currentIndex()].id;
    tmp.plantInfo_id = type_pos[ui->type_combobox->currentIndex()].id;
    tmp.moisture_set = ui->moisture_slider->value();
    tmp.tmp_set = ui->temperature_slider->value();
    tmp.rotate_set = ui->rotate_slider->value();

    ctlPanel_->changePlant(tmp);

    updateParameter_slider(tmp);
}

void Touchscreen::on_default_button_clicked()
{

    PlanteInfo pI = ctlPanel_->getPlantInfo(type_pos[ui->type_combobox->currentIndex()].id);
    PlantValues tmp;
    tmp.id = plant_pos[ui->plant_combobox->currentIndex()].id;
    tmp.plantInfo_id = pI.id;
    tmp.moisture_set = pI.moisture;
    tmp.tmp_set = pI.tmp;
    tmp.rotate_set = pI.rotate;

    ctlPanel_->changePlant(tmp);

    updateParameter_slider(pI);
}

void Touchscreen::on_pushButton_clicked()
{
    ctlPanel_->removePlant(plant_pos[ui->plant_combobox->currentIndex()].id);
}

void Touchscreen::on_type_combobox_activated(int index)
{
    PlanteInfo pi = ctlPanel_->getPlantInfo(type_pos[index].id);

    updateParameter_slider(pi);
}

void Touchscreen::on_plant_combobox_activated(int index)
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
    updateParameter_slider(pv);

}

void Touchscreen::on_moisture_slider_valueChanged(int value)
{
    ui->moisture_label->setText(QString("Moisture ") + QString::number(value) + "%");
}

void Touchscreen::on_temperature_slider_valueChanged(int value)
{
    ui->temperature_label->setText(QString("Max temperature ") + QString::number(value)+ "C");
}

void Touchscreen::on_rotate_slider_valueChanged(int value)
{
    ui->rotate_label->setText(QString("Rotate ") + QString::number(value) + " deg/hour");
}

//--------test side

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

