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
    WiFi wf;
    Touchscreen ui;

    //database testdb;
    //tcpSocket* testtcp = new tcpSocket(&testdb, 8888);

    //QThreadPool::globalInstance()->start(testtcp);

    Controlpanel(&pd, &pl, &ui, &wf, false);

    return a.exec();
}
