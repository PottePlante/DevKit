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

    Planteliste pl;
    PlanteDatabase pd(pl.getMutex());
    //WiFi wf;
    Touchscreen ui;
    WiFi wf;

    Controlpanel ctl(&pd, &pl, &ui, &wf, false);
    //QThreadPool::globalInstance()->start(wf); bruger QObject til at køre en anden tråd med.

    return a.exec();
}
