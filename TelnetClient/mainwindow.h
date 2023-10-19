#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readFromTom();
    void onDisconnected();

private slots:
    void on_start_clicked();
    void on_stop_clicked();
    void on_send_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket=nullptr;
};

#endif // MAINWINDOW_H
