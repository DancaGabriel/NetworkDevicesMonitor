#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMessageBox>
#include "mainwindow.h"
#include "mainpage.h"
#include <QFile>
#include <QDebug>
#include "shared.h"
#include "worker.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    SharedData data;

public slots:
    void handleMessage(const QString&  message);

signals:
    void signalToServerSomething(const QString& mesaj);
    void trimitLaServer(const QString& user);


private slots:
    void on_pushButton_clicked();

    //void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void on_usernameReturnPressed();




private:
    Ui::Login *ui;
};

#endif // LOGIN_H
