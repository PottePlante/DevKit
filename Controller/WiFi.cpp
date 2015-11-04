#include "WiFi.h"
using namespace std;

WiFi::WiFi(int port)
{
    port_ = port;
}

void WiFi::init(Controlpanel * CP)
{
    ctlPanel_ = CP;
}

void WiFi::run()
{
    int socket_desc, c ,read_size;
    struct sockaddr_in server, client;
    char client_message[30];

    //Creating socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
        qDebug() <<"Could not create socket\n";
    else
        qDebug() << "Socket created\n";

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;            //Listen interface
    server.sin_addr.s_addr = INADDR_ANY;    //Listen adress
    server.sin_port = htons(port_);         //Listen port

    //Bind to networkcard
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        qDebug() << "bind failed. Error\n";
    else
        qDebug() << "bind done\n";

    //Start listen for incoming connection
    listen(socket_desc, 1);

    c = sizeof(struct sockaddr_in);

    for(;;)
    {
        qDebug() << "Waiting for incoming connections...\n";

        //Accept connection from an incoming client
        client_sock_ = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock_ < 0)
            qDebug() << "accept failed";
        else
            qDebug() << "Connection accepted";

        //Receive a message from client
        while( (read_size = recv(client_sock_ , client_message , 30 , 0)) > 0 )
        {
            handler(client_message);
        }

    }
}

void WiFi::handler(char* cMsg)
{
    qDebug() << "WiFi handler start";
    PlantValues PV;
    QString data_num;
    int id;

    data_num = cMsg[1];
    id = atoi(data_num.toStdString().c_str());
    if(id >=0)
        PV.id = id;
    else
        PV.id = -1;

    qDebug() << "WiFi handler id: " << PV.id;

    data_num = cMsg[2] + cMsg[3] + cMsg[4];
    qDebug() << data_num;
    PV.moisture = atoi(data_num.toStdString().c_str());

    qDebug() << "WiFi handler moisture: " << PV.moisture;

    data_num = cMsg[7] + cMsg[8] + cMsg[9];
    PV.water = atoi(data_num.toStdString().c_str());

    qDebug() << "WiFi handler water: " << PV.water;

    data_num = cMsg[12] + cMsg[13] + cMsg[14];
    PV.light = atoi(data_num.toStdString().c_str());

    qDebug() << "WiFi handler light: " << PV.light;

    data_num = cMsg[17] + cMsg[18] + cMsg[19];
    PV.tmp = atoi(data_num.toStdString().c_str());

    qDebug() << "WiFi handler tmp: " << PV.tmp;

    data_num = cMsg[22] + cMsg[23] + cMsg[24];
    PV.battery = atoi(data_num.toStdString().c_str());

    qDebug() << "WiFi handler battery: " << PV.battery;

    ctlPanel_->updatePlantValue(PV);
    qDebug() << "WiFi handler done";
}

void WiFi::update(PlantValues PV)
{
    qDebug() << "WiFi update start";
    char data_send[30];
    char data[3];
    char id[2];

    sprintf(id, "%d", PV.id);

    data_send[0] = 'M';
    data_send[1] = id[0];

    data_send[5] = 'R';
    data_send[6] = id[0];


    sprintf(data, "%d", PV.moisture_set);
    if(PV.moisture_set > 99)
    {
        data_send[2] = data[0];
        data_send[3] = data[1];
        data_send[4] = data[2];
    }
    else
    {
        data_send[2] = '0';
        data_send[3] = data[0];
        data_send[4] = data[1];
    }


    sprintf(data, "%d", PV.rotate_set);
    if(PV.rotate_set > 99)
    {
        data_send[7] = data[0];
        data_send[8] = data[1];
        data_send[9] = data[2];
    }
    else
    {
        data_send[7] = '0';
        data_send[8] = data[0];
        data_send[9] = data[1];
    }

    qDebug() << "Sender til PSoC :)";

    for(int i=0; i<30; i++)
        qDebug() << data_send[i];

    write(client_sock_, data_send, 30);

}

