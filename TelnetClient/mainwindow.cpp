#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Display1->setPlaceholderText(" Type message to chat");
    connect(ui->Display1, &QLineEdit::returnPressed, this, &MainWindow::on_send_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 9999);
    //socket->connectToHost("10.240.8.253", 9999);

    if (socket->waitForConnected(500))
    {
        qDebug() << "Client Connected!";
        socket->write("Tom");
        socket->flush();
        socket->waitForBytesWritten(500);
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readFromTom);
        connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
        ui->Display2->append("Client Connected.");
    }
    else
    {
        ui->Display2->append("Server not found!");
        qDebug() << "Not connected!";
    }
}

void MainWindow::readFromTom()
{
    QByteArray message = socket->readAll();
    ui->Display2->append("Server: "+message);
    //qDebug() << message;
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected from server";
    ui->Display2->append("Server closed connection!");
}

void MainWindow::on_send_clicked()
{
    if(socket==nullptr || socket->state() != QAbstractSocket::ConnectedState)
    {
        ui->Display2->append("Server not found!");
        ui->Display1->clear();
    }
    else {
        QString message = ui->Display1->text();
        socket->write(message.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(500);
        ui->Display1->clear();
    }
}

void MainWindow::on_stop_clicked()
{
    if (socket!=nullptr && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
        ui->Display2->append("Chatting Closed.");
    }
    else
    {
        ui->Display2->append("Chatting not started yet.");
    }
}
