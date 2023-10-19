#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serversockethandler.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();

    sendMessage(socket,"Hello Client, Server here");

    auto handler = new ServerSocketHandler(socket);
    handler->show();

    QString var = socket->peerAddress().toString()+"."+QString::number(socket->peerPort());
    //qDebug() << "Client identifer = " << var;
    clientList.insert(var, handler);

    connect(socket, &QTcpSocket::readyRead, handler, &ServerSocketHandler::readFromClient);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    //qDebug() << clientList[var]->getName();
    ui->comboBox->addItem(var);
}


void MainWindow::on_start_clicked()
{
    if(server != nullptr && server->isListening())
    {
        ui->Display2->append("Server is already running.");
    }
    else
    {
        server = new QTcpServer(this);
        if(!server->listen(QHostAddress::Any, 9999))
        {
            qDebug() << "Server could not start";
            ui->Display2->append("Server could not start");
        }
        else
        {
            qDebug() << "Server started!";
            ui->Display2->append("Server Started.");
            connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        }
    }
}

void MainWindow::on_stop_clicked()
{
    if (server!=nullptr && server->isListening())
    {
        server->close();
        ui->Display2->append("Server Closed.");
    }
    else
    {
        ui->Display2->append("Server not started yet.");
    }
}

void MainWindow::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "inside Disconnect";
    if(socket==nullptr) {
        return;
    }
    QString var = socket->peerAddress().toString()+"."+QString::number(socket->peerPort());
    //qDebug() << var;
    auto it = clientList.begin();
    for(;it!=clientList.end();it++)
    {
        if(it.key() == var)
        {
            if(it.value())
            {
                it.value()->close();
                it.value()->~ServerSocketHandler();
            }
            clientList.erase(it);
            int index = ui->comboBox->findText(var);
            ui->comboBox->removeItem(index);
            return;
        }
    }
}

void MainWindow::sendMessage(QTcpSocket *socket,QByteArray message)
{
    QString var = socket->peerAddress().toString()+"."+QString::number(socket->peerPort());
    qDebug() <<var;
    //auto name = clientList[var]->getName();
//    auto flag = clientList[var]->getFlag();
//    if(flag == false && name =="Unknown")
//    {
//        clientList[var]->setFlag(true);
//        int index = ui->comboBox->findText(var);
//        ui->comboBox->removeItem(index);
//        ui->comboBox->addItem(name);
//    }

    socket->write(message);
    socket->flush();
    socket->waitForBytesWritten(500);
}
