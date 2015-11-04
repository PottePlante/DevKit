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
        qDebug() <<"Could not create socket";
    else
        qDebug() << "Socket created";

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;            //Listen interface
    server.sin_addr.s_addr = INADDR_ANY;    //Listen adress
    server.sin_port = htons(port_);         //Listen port

    //Bind to networkcard
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        qDebug() << "bind failed. Error";
    else
        qDebug() << "bind done";

    //Start listen for incoming connection
    listen(socket_desc, 1);

    c = sizeof(struct sockaddr_in);

    for(;;)
    {
        qDebug() << "Waiting for incoming connections...";

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
    char tmp[4];

    data_num = cMsg[1];
    id = data_num.toInt();
    if(id >=0)
        PV.id = id;
    else
        PV.id = -1;

    qDebug() << "WiFi handler id: " << PV.id;

    tmp[0] = cMsg[2];
    tmp[1] = cMsg[3];
    tmp[2] = cMsg[4];
    tmp[3] = '\0';

    data_num = tmp;
    qDebug() << data_num;
    PV.moisture = data_num.toInt();

    qDebug() << "WiFi handler moisture: " << PV.moisture;

    tmp[0] = cMsg[7];
    tmp[1] = cMsg[8];
    tmp[2] = cMsg[9];
    tmp[3] = '\0';
    data_num = tmp;
    PV.water = data_num.toInt();

    qDebug() << "WiFi handler water: " << PV.water;

    tmp[0] = cMsg[12];
    tmp[1] = cMsg[13];
    tmp[2] = cMsg[14];
    tmp[3] = '\0';
    data_num = tmp;
    PV.light = data_num.toInt();

    qDebug() << "WiFi handler light: " << PV.light;

    tmp[0] = cMsg[17];
    tmp[1] = cMsg[18];
    tmp[2] = cMsg[19];
    tmp[3] = '\0';
    data_num = tmp;
    PV.tmp = data_num.toInt();

    qDebug() << "WiFi handler tmp: " << PV.tmp;

    tmp[0] = cMsg[22];
    tmp[1] = cMsg[23];
    tmp[2] = cMsg[24];
    tmp[3] = '\0';
    data_num = tmp;
    PV.battery = data_num.toInt();

    qDebug() << "WiFi handler battery: " << PV.battery;

    ctlPanel_->updatePlantValue(PV);
    qDebug() << "WiFi handler done";
}

void WiFi::update(PlantValues PV)
{
    qDebug() << "WiFi update start";
    char data_send[10];
    char data[3];
    char id[2];

    sprintf(id, "%d", PV.id);

    data_send[0] = 'M';
    data_send[1] = id[0];

    data_send[5] = 'R';
    data_send[6] = id[0];


    sprintf(data, "%d", PV.moisture_set);
    qDebug() << "Moisture_set: " <<PV.moisture_set << "=" << data[0] << data[1] << data[2];
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

    write(client_sock_, data_send, 10);

}

