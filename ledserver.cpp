#include "ledserver.h"
#include "config.h"

LedServer::LedServer(quint16 port, QObject *parent) : QObject(parent), m_port(port)
{
    m_server = new QTcpServer(this);
        connect(m_server, &QTcpServer::newConnection,
                this, &LedServer::myNewConnection);
}

void LedServer::start()
{
    m_gpio = new Gpio(this);
    m_server->listen(QHostAddress::Any, PORT);
    qDebug() << "Server lauscht auf den Port: " << PORT;
}

// Client hat Verbindung zum Server aufgebaut
void LedServer::myNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    connect(m_socket,&QTcpSocket::readyRead,
            this,&LedServer::myServerRead);
    connect(m_socket,&QTcpSocket::disconnected,
            this,&LedServer::myClientDisconnect);
    qDebug() << "Neue Client-Verbindung!";
}

// Client hat Verbindung zum Server getrennt
void LedServer::myClientDisconnect()
{
    m_socket->close();
    m_gpio->set(0000);
    qDebug() << "Client hat die Verbindung getrennt!";
}

// Client hat eine Zahl (0...15) zum Server gesendet
void LedServer::myServerRead()
{
    QString message = m_socket->readAll();

    int msg = message.toInt();

        if(msg >= 0 && msg <= 15)
        {
            m_socket->write("Ok\n");
            m_gpio->set(msg);
            m_socket->write("Geben Sie eine Zahl von 0 bis 15 ein: \n");
        }
        else {
            m_socket->write("Geben Sie eine Zahl von 0 bis 15 ein: \n");
        }
            // :D
}
