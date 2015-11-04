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
    char client_message[5];

    //Creating socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
        qDebug() <<"Could not create socket\n";
    else
        qDebug() << "Socket created\n";

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;            //Listen interface
    server.sin_addr.s_addr = INADDR_ANY;    //Listen adress
    server.sin_port = htons(port_);         //Lisen port

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
        while( (read_size = recv(client_sock_ , client_message , 5 , 0)) > 0 )
        {
            handler(client_message);
        }

    }
}

void WiFi::handler(char* cMsg)
{
    PlantValues PV;
    PV.id = cMsg[1];

    QString data_num;
    data_num = cMsg[2] + cMsg[3] + cMsg[4];

    int temp = atoi(data_num.toStdString().c_str());

    switch (cMsg[0])            //kommer ikke til at virke!!!
    {
    case 'M':
    {
        PV.moisture = temp;
        ctlPanel_->updatePlantValue(PV);
        toSend_ = 'M';
        break;
    }
    case 'W':
    {
        PV.water = temp;
        ctlPanel_->updatePlantValue(PV);
        toSend_ = 'W';
        break;
    }
    case 'L':
    {
        PV.light = temp;
        ctlPanel_->updatePlantValue(PV);
        toSend_ = 'L';
        break;
    }
    case 'T':
    {
        PV.tmp = temp;
        ctlPanel_->updatePlantValue(PV);
        toSend_ = 'T';
        break;
    }
    case 'B':
    {
        PV.battery = temp;
        ctlPanel_->updatePlantValue(PV);
        toSend_ = 'B';
        break;
    }
    case 'R':
    {
        PV.battery = temp;
        ctlPanel_->updatePlantValue(PV);
        toSend_ = 'R';
        break;
    }
    default:
        break;
    }
}

void WiFi::update(PlantValues PV)
{
    char data_send[5];

    char id[2];
    sprintf(id, "%d", PV.id);
    data_send[0] = toSend_;
    data_send[1] = id[0];


    switch (toSend_) {
    case 'M':
    {
        char data[3];
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

        write(client_sock_, data_send, 5);

        break;
    }
    case 'R':
    {
        char data[3];
        sprintf(data, "%d", PV.rotate_set);

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

        write(client_sock_, data_send, 5);

        break;
    }
    default:
    {
        data_send[2] = '0';
        data_send[3] = '0';
        data_send[4] = '0';

        write(client_sock_, data_send, 5);
        break;
    }
    }

}

