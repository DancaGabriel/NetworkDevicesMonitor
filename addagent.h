#ifndef ADDAGENT_H
#define ADDAGENT_H

#include <QMainWindow>
#include "mainpage.h"
#include <QDebug>
#include "shared.h"
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include "login.h"
#include <QRegularExpression>


namespace Ui {
class AddAgent;
}

class AddAgent : public QMainWindow
{
    Q_OBJECT
public:
    //explicit AddAgent(QObject *parent = nullptr);
    explicit AddAgent(QWidget *parent = nullptr);
     ~AddAgent();
    QString adresa_ip;


protected:
    void run();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public slots:
    void handleSemnal(const QString& mesaj);
    void handleMessageFromServer(const QString& mesaj);
    void arataIP(const QString& ip);



signals:
    void serverResponseReceived(const QString& response);
    void signalToServerIP(const QString& ip);
    void SemnalIp(const QString &ip_adress);


private:
    Ui::AddAgent *ui;
    void client_function();

};

#endif // ADDAGENT_H
