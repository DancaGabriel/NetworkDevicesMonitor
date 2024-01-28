#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include <string.h>
#include "addagent.h"
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QRegularExpression>
#include <QThread>
#include "addagent.h"
#include "worker.h"



// extern int errno;
// #define DEFAULT_PORT 2908
// #define SERVER_IP "127.0.0.1"

// typedef struct {
//     int status;
//     char message[4096];
// } ServerResponse;


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainwindow;

    QThread::currentThread()->setObjectName("Main Thread");

    Login login;
    login.show();

    Worker *workerThread = new Worker();
    workerThread->setObjectName("Sclavul");

    qInfo() << "Starting" << workerThread->objectName();
    workerThread->start();
    int result = a.exec();
    workerThread->quit();
    workerThread->wait();
    delete workerThread;

    return result;
}

