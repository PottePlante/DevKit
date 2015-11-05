#ifndef WIFI_H
#define WIFI_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <QDebug>
#include <QString>
#include <QObject>
#include <QThread>
#include "Planteliste.h"
#include "PlanteDatabase.h"

class Controlpanel;

class WiFi : public QObject
{
    Q_OBJECT
public:
    WiFi(int port = 8888);

    void init(Controlpanel * CP);

    void update(PlantValues);

private slots:
    void run();
//signals:
//    void newData(PlantValues);
private:
    Controlpanel* ctlPanel_;
    int port_;
    int client_sock_;
    void handler(char* cMsg);
};

#endif // WIFI_H


