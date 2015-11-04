#include "plantitems_ui.h"
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QImage>
#include <QImageReader>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "PlanteDatabase.h"
#include "Planteliste.h"


PlantItems_ui::PlantItems_ui(PlanteInfo pI, PlantValues pV)
    :id_(-1)
{

    if(pV.id == -1)
        return;

    id_ = pV.id;

    box_ = new QGroupBox(pI.name + " id: " + QString::number(pV.id));

    box_->setFixedWidth(120);
    //box_->setFixedHeight(50);


    water_ = new QProgressBar();
    water_->setMaximum(100);
    water_->setMaximumHeight(10);
    water_->setValue(pV.water);
    text_ = new QLabel("no info");

    QImageReader reader(pI.image);

    reader.setAutoDetectImageFormat(true);
    image_ = new QImage(reader.read());
    view_ = new QGraphicsView();
    scene_ = new QGraphicsScene();

    layout_ = new QVBoxLayout;
    if(!image_->isNull())
    {
        scene_->addPixmap(QPixmap::fromImage(*image_));

        view_->setScene(scene_);
        view_->scale(0.2, 0.2);

        layout_->addWidget(view_);

//        image_txt_->setPixmap(QPixmap::fromImage(*image_));
//        layout_->addWidget(image_txt_);
    }

    layout_->addWidget(water_);
    layout_->addWidget(text_);
    //layout_->addStretch(1);

    box_->setLayout(layout_);


}

PlantItems_ui::~PlantItems_ui()
{
    delete layout_;
    delete box_;
    delete text_;
    delete water_;
}

int PlantItems_ui::getId() const
{
    return id_;
}

QGroupBox* PlantItems_ui::get() const
{
    return box_;
}

bool PlantItems_ui::update(PlanteInfo pI, PlantValues pV)
{
    if(pV.id != id_)
        return false;

    box_->setTitle(pI.name + " id: " + QString::number(pV.id));

    water_->setValue(pV.water);
    return true;
}

