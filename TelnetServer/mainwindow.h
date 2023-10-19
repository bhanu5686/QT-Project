#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTextEdit>
#include <QLineEdit>
#include <QDebug>
#include <QWidget>
#include <QComboBox>
#include "serversockethandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readFromClient();
    void onDisconnected();
    void sendMessage(QTcpSocket *,QByteArray);

signals:

public slots:
    void newConnection();

private slots:
    void on_start_clicked();
    void on_stop_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server=nullptr;
    QMap<QString, ServerSocketHandler *> clientList;
};

#endif
