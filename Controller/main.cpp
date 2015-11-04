#include "touchscreen.h"
#include "Controlpanel.h"
#include "PlanteDatabase.h"
#include "Planteliste.h"
#include "WiFi.h"
#include <QApplication>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlanteDatabase pd;
    Planteliste pl;
    //WiFi wf;
    Touchscreen ui;

    WiFi* wf = new WiFi();

    Controlpanel ctl(&pd, &pl, &ui, wf, false);
    QThreadPool::globalInstance()->start(wf);

    return a.exec();
}
