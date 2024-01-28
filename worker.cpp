#include "worker.h"
#include <QDebug>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <strings.h>
#include <stdio.h>


#define DEFAULT_PORT 2908
#define SERVER_IP "127.0.0.1"

void Worker::run() {
    client_function();
}

 void Worker::handleSignalIP(const QString &ip)
 {
     qDebug() << "Am primit ip-ul: " << ip;
     emit sendIPtoServer(ip);

 }

 void Worker::handleUnMEsaj(const QString &mesaj)
 {
     qDebug() << "Am primit mesajul de la addagent: " << mesaj;
 }



 Worker::Worker(QObject *parent) : QThread(parent) {
     if ((sd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         perror("Eroare la socket().\n");
     }

     struct sockaddr_in server;
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = inet_addr(SERVER_IP);
     server.sin_port = htons(DEFAULT_PORT);

     if (::connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
         perror("[client]Eroare la connect().\n");
         ::close(sd);
     }
     connect(this, &Worker::sendIPtoServer, this, &Worker::sendIpRemote);
 }


void Worker::client_function() {

    bool ok1 = true;
    int nr;
    char mesaj_primit[4096];

   //  while (1) {
   //      if (ok1 == true) {
   //          char text[100]="nimic, doar de proba :)"; // adica nu conteaza ce scriu aici

   //          qDebug("[client] Am citit %s", text);

   //          if (::write(sd, text, sizeof(text)) <= 0) {
   //              qCritical("[client]Eroare la write() spre server: %s", strerror(errno));
   //          }
   //          bzero(mesaj_primit,sizeof(mesaj_primit));

   //          if (::read(sd, mesaj_primit, sizeof(mesaj_primit)) < 0) {
   //              qCritical("[client]Eroare la read() de la server: %s", strerror(errno));
   //          }

   //          qDebug("[client]Mesajul primit este: %s", mesaj_primit);
   //          QString mesajPrimitQString = QString::fromUtf8(mesaj_primit);
   //          this->adresa_ip = mesajPrimitQString;
   //          qDebug() << this->adresa_ip;
   //          ok1 = false;
   //      }
   // }

    //close(sd);
}
// slot care trimite din IP-ul adaugat din AddAgent
void Worker::sendIpRemote(const QString& ip) {
    char mesaj_primit[4096];

    qDebug()<< "suntem in functia care trimite la server";

    QByteArray byteArray = "add_ip " + ip.toUtf8();
    const char* text = byteArray.constData();

    qDebug("[client] Am citit %s", text);

    if (::write(sd, text, byteArray.size()) <= 0) {
        qCritical("[client]Eroare la write() spre server: %s", strerror(errno));
    }
    bzero(mesaj_primit, sizeof(mesaj_primit));

    if (::read(sd, mesaj_primit, sizeof(mesaj_primit)) < 0) {
        qCritical("[client]Eroare la read() de la server: %s", strerror(errno));
    }
    qDebug("[client]Mesajul primit este: %s", mesaj_primit);
    QString mesajPrimitQString = QString::fromUtf8(mesaj_primit);
    if (mesajPrimitQString.contains("nu")) {
        qDebug("Mesajul contine 'nu'.");
        emit sendResponseToAddAgent("nu"); // IP-ul introdus de utilizator este valid, dar nu exista in server, adica nu a fost adaugat
    } else {
        emit sendResponseToAddAgent("da"); // exista in server
    }

}
// slot care primeste ce tipuri de log-uri doreste ueser-ul
void Worker::handleRequesFromSystem(const QString adresa_ip, const QString& type_of, const QString& quantity, const QString& period)
{
    qDebug()<< "Am primit in worker de la System: " << adresa_ip << " " << type_of << " " << quantity << " " << period;
    char mesaj_primit[9086];

    QByteArray byteArray= "request_logs " + adresa_ip.toUtf8() + " " + type_of.toUtf8() + " " + quantity.toUtf8() + " " + period.toUtf8();

    const char* text = byteArray.constData();

    qDebug("[client] Am citit %s", text);

    if (::write(sd, text, byteArray.size()) <= 0) {
        qCritical("[client]Eroare la write() spre server: %s", strerror(errno));
    }
    bzero(mesaj_primit, sizeof(mesaj_primit));

    if (::read(sd, mesaj_primit, sizeof(mesaj_primit)) < 0) {
        qCritical("[client]Eroare la read() de la server: %s", strerror(errno));
    }
    qDebug("[client]Mesajul primit este: %s", mesaj_primit);
    QString mesajPrimitQString = QString::fromUtf8(mesaj_primit);

    emit sendResponseToSystem(mesajPrimitQString);

}

void Worker::primimLogin()
{
    qDebug() << "Cerem adresa IP a serverului!";
    char mesaj_primit[4096];
    QByteArray byteArray = "doresc_ip";
    const char* text = byteArray.constData();
    if (::write(sd, text, byteArray.size()) <= 0) {
        qCritical("[client]Eroare la write() spre server: %s", strerror(errno));
    }
    bzero(mesaj_primit, sizeof(mesaj_primit));

    if (::read(sd, mesaj_primit, sizeof(mesaj_primit)) < 0) {
        qCritical("[client]Eroare la read() de la server: %s", strerror(errno));
    }
    qDebug("[client]Mesajul primit este: %s", mesaj_primit);
    QString mesajPrimitQString = QString::fromUtf8(mesaj_primit);
    emit sendIP(mesajPrimitQString);
}

void Worker::primimUsername(const QString &username)
{
    qDebug() << "username-ul este in Worker" << username;
    QFile file("ip_list.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       //QMessageBox::critical(this, "Error", "Could not open user file");
    }
    QString adresa_ip;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ");

        if (parts.size() == 2 && parts[0] == username) {
            adresa_ip = parts[1];
            break;
        }
    }
    file.close();
    qDebug() << "ip este in Worker" << adresa_ip;
     char mesaj_primit[4096];
     QByteArray byteArray = "banana " + adresa_ip.toUtf8();
     const char* text = byteArray.constData();
     if (::write(sd, text, byteArray.size()) <= 0) {
         qCritical("[client]Eroare la write() spre server: %s", strerror(errno));
     }
     bzero(mesaj_primit, sizeof(mesaj_primit));

     if (::read(sd, mesaj_primit, sizeof(mesaj_primit)) < 0) {
         qCritical("[client]Eroare la read() de la server: %s", strerror(errno));
     }
    qDebug("[client]Mesajul primit este: %s", mesaj_primit);
     emit sendSecurityIncidents(mesaj_primit);

}

