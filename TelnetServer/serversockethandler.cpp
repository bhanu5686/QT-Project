#include "serversockethandler.h"
#include "ui_serversockethandler.h"

ServerSocketHandler::ServerSocketHandler(QTcpSocket *clientSocket, QWidget *parent) : QDialog(parent), ui(new Ui::ServerSocketHandler)
{
    this->clientSocket = NULL;
    this->clientName = "Unknown";
    this->ui->setupUi(this);
    this->clientSocket = clientSocket;
    this->ui->Display1->setPlaceholderText(" Type message to chat");
    this->flag = false;
    connect(this->ui->Display1, &QLineEdit::returnPressed, this, &ServerSocketHandler::on_send_clicked);
}

ServerSocketHandler::~ServerSocketHandler()
{
    delete this->ui;
}

QTcpSocket *ServerSocketHandler::getClientSocket()
{
    return this->clientSocket;
}

void ServerSocketHandler::setName(QByteArray message)
{
    this->clientName = message;
}

QByteArray ServerSocketHandler::getName()
{
    return this->clientName;
}

bool ServerSocketHandler::getFlag()
{
    return this->flag;
}

bool ServerSocketHandler::setFlag(bool flag)
{
    this->flag= flag;
}

void ServerSocketHandler::readFromClient()
{
    auto socket = getClientSocket();
    qDebug() << socket->localPort();
    QByteArray message = socket->readAll();
    if(flag)
    {
        this->setName(message);
        this->ui->Display2->append("Hello "+message);
        flag = false;
    }
    else
    {
        this->ui->Display2->append("Client: "+message);
    }
}

void ServerSocketHandler::on_send_clicked()
{
    auto socket = getClientSocket();
    if(socket!=nullptr && socket->state() == QAbstractSocket::ConnectedState)
    {
        QString message = this->ui->Display1->text();
        socket->write(message.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(500);
        this->ui->Display1->clear();
    }
    else {
        this->ui->Display2->append("Client not found!");
        this->ui->Display1->clear();
    }
}

void ServerSocketHandler::on_end_clicked()
{
    auto socket = getClientSocket();
    if (socket!=nullptr && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
    }

    clientSocket=NULL;
    delete clientSocket;
}
