#ifndef WIFI_H
#define WIFI_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <QtCore>
#include <QDebug>
#include <QString>
#include "Controlpanel.h"
#include "Planteliste.h"
#include "PlanteDatabase.h"

struct PlantValues;

class WiFi : public QRunnable
{
public:
    WiFi(int port = 8888);

    void init(Controlpanel * CP);
    void run();
    void update(PlantValues);

private:
    Controlpanel* ctlPanel_;
    char * buf_;
    int busSize_;
    int port_;
    int client_sock_;
    char toSend_;
    void handler(char* cMsg);
};

#endif // WIFI_H


