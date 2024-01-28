#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "addagent.h"
#include <QMutex>
#include <QMessageBox>

class Worker : public QThread {
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    bool add_ip = false;
    QString ip_adaugat;
    QMutex mutex;
    int sd;
    QString adresa_ip;


signals:
    void signalMessageReceived(const QString& message);
    void sendIPtoServer(const QString& ip);
    void sendResponseToAddAgent(const QString& mesaj);
    void sendResponseToSystem(const QString& mesaj);
    void sendIPtoDashboard(const QString& ip);
    void sendIP(const QString& mesaj);
    void sendSecurityIncidents(const QString& mesaj);

public slots:
    void handleSignalIP(const QString &ip);
    void handleUnMEsaj(const QString &mesaj);
    void sendIpRemote(const QString &ip);
    void handleRequesFromSystem(const QString ip,const QString& type_of,const QString& quantity, const QString& period);
    void primimLogin();
    void primimUsername(const QString& username);

protected:
    void run() override;
    void client_function();
};

#endif // WORKER_H
