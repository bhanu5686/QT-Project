#ifndef SERVERSOCKETHANDLER_H
#define SERVERSOCKETHANDLER_H

#include <QDialog>
#include <QObject>
#include <QTcpSocket>

namespace Ui {
class ServerSocketHandler;
}

class ServerSocketHandler : public QDialog
{
Q_OBJECT

public:
    explicit ServerSocketHandler(QTcpSocket *clientSocket, QWidget *parent = nullptr);
    ~ServerSocketHandler();
    QTcpSocket *getClientSocket();
    void setName(QByteArray);
    QByteArray getName();
    bool getFlag();
    bool setFlag(bool);

signals:

public slots:
    void readFromClient();

private slots:
    void on_send_clicked();
    void on_end_clicked();

private:
    Ui::ServerSocketHandler *ui;
    QTcpSocket *clientSocket;
    QByteArray clientName;
    bool flag;
};

#endif
