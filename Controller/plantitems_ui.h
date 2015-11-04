#ifndef PLANTITEMS_UI_H
#define PLANTITEMS_UI_H

class QGridLayout;
class QGroupBox;
class QLabel;
class QProgressBar;
class QVBoxLayout;
class QImage;
class QGraphicsView;
class QGraphicsScene;


struct PlantValues;
struct PlanteInfo;

class PlantItems_ui
{
public:
    PlantItems_ui(PlanteInfo, PlantValues);
    ~PlantItems_ui();

    int getId() const;
    QGroupBox* get() const;

    bool update(PlanteInfo, PlantValues);

private:
    int id_;
    QVBoxLayout* layout_;
    QImage* image_;
    QGraphicsScene* scene_;
    QGraphicsView* view_;
    QGroupBox* box_;
    QLabel* text_;
    QProgressBar* water_;
};

#endif // PLANTITEMS_UI_H
