#include "WiFi.h"
#include "Controlpanel.h"

using namespace std;

WiFi::WiFi(int port):
    port_(port)
{}

void WiFi::init(Controlpanel * CP)
{
    ctlPanel_ = CP;
}

void WiFi::run()
{
    int c ,read_size;
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
    if(!cMsg[1] == '-')
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
    QByteArray Sdata = "M";

    Sdata.append(QByteArray::number(PV.id));

    if(PV.moisture_set > 99)
        Sdata.append(QByteArray::number(PV.moisture_set));

    else if(PV.moisture_set < 0)
        Sdata.append("err");

    else
    {
        Sdata.append('0');
        Sdata.append(QByteArray::number(PV.moisture_set));
    }

    Sdata.append("R");
    Sdata.append(QByteArray::number(PV.id));

    qDebug() << "rotate_set modtaget der skal sendes: " << PV.rotate_set;
    if(PV.rotate_set > 99)
        Sdata.append(QByteArray::number(PV.rotate_set));

    else if(PV.rotate_set < 0)
        Sdata.append("err");

    else
    {
        Sdata.append('0');
        Sdata.append(QByteArray::number(PV.rotate_set));
    }

    qDebug() << "Sender til PSoC :)" << Sdata;


    char* dataToSend = Sdata.data();
    //Sender
    write(client_sock_, dataToSend, strlen(dataToSend));

}
