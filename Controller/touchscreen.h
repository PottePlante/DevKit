#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <QMainWindow>
#include <vector>
#include "PlanteDatabase.h"
#include "Planteliste.h"


//prototyper
//struct PlantValues;
//struct PlanteInfo;

struct ui_plant_item;
class Controlpanel;
class QSpacerItem;
class PlanteDatabase;
// -------

class PlantItems_ui;

struct pos_combo{
    int id;
    int pos;
};

namespace Ui {
class Touchscreen;
}

class Touchscreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit Touchscreen(QWidget *parent = 0);
    ~Touchscreen();
    void init(Controlpanel*, std::vector<PlanteInfo> pI_vec, std::vector<PlantValues> pV_vec);

private slots:

    void update(PlanteInfo, PlantValues);

    void on_test_add_clicked();

    void on_get_status_button_clicked();

    void on_plant_combobox_currentIndexChanged(int index);

private:
    void updateCombobox();
    void updateStatusPage();
    void updateParameter();
    std::vector<pos_combo> plant_pos;
    std::vector<pos_combo> type_pos;
    std::vector<PlantItems_ui*> list_;
    Ui::Touchscreen *ui;
    Controlpanel* ctlPanel_;
    QSpacerItem* space_;
};

#endif // TOUCHSCREEN_H
