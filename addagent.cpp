#include "addagent.h"
#include "ui_addagent.h"
#include "shared.h"
#include "worker.h"
#include <QThread>

 AddAgent::AddAgent(QWidget *parent)
      : QMainWindow(parent)
     , ui(new Ui::AddAgent)
  {
     ui->setupUi(this);
     Worker *worker11 = new Worker();
     connect(this, &AddAgent::SemnalIp, worker11, &Worker::handleSignalIP, Qt::QueuedConnection);
     connect(worker11,&Worker::sendResponseToAddAgent,this,&AddAgent::handleMessageFromServer,Qt::QueuedConnection);
     connect(worker11,&Worker::sendIPtoDashboard,this,&AddAgent::arataIP,Qt::QueuedConnection);
}


 AddAgent::~AddAgent() {
     //delete ui;
 }


void AddAgent::on_pushButton_clicked() {

    QString ip_address = ui->lineEdit->text();
    ui->lineEdit->clear();
    qDebug() << "Username-ul in addAgent este" << SharedData::getUsername();
    QRegularExpression ipRegex("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");

    if (!ipRegex.match(ip_address).hasMatch()) {
        qDebug() << "Formatul adresei IP este invalid!";
        QMessageBox::information(this, "AddAgent", "Invalid IP format, try again!");
    }
    else {
        QFile file("ip_list.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Could not open user file");
            return;
        }

        QTextStream in(&file);
        bool ip_exists = false;

         while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");

            if (parts.size() == 2 && parts[0] == SharedData::getUsername() && parts[1] == ip_address) {
                ip_exists = true;
                break;
             }
         }
        file.close();
         if(ip_exists == true){
             QMessageBox::information(this, "AddAgent", "The IP adress has been added already, add another agent or go and monitor your device!");
        }
         else{
             this->adresa_ip = ip_address;
           qDebug() << "Emitting signalToServerIP with IP address:" << ip_address;
           emit SemnalIp(ip_address);
        }
    }
}


void AddAgent::on_pushButton_2_clicked()
{
    this->hide();
    MainPage *main = new MainPage();
    main->show();
}

void AddAgent::handleSemnal(const QString &mesaj)
{
    qDebug("sunt in addagent");
}

void AddAgent::handleMessageFromServer(const QString &mesaj)
{
    qDebug() <<"Am primit de la Worker mesajul: " << mesaj;
    if(mesaj.contains("nu") == true){
         QMessageBox::information(this, "AddAgent", "The IP adress was not found,try again!");
    }
    else{
        QFile file("ip_list.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Could not open user file");
            return;
        }

        QTextStream in(&file);
        in.seek(0);
        in << SharedData::getUsername() << " " << this->adresa_ip << "\n";
        file.close();
        QMessageBox::information(this, "AddAgent", "The IP adress was found!");
    }


}

void AddAgent::arataIP(const QString &ip)
{
    qDebug() << "add agent ip " << ip;
}

