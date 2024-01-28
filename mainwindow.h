#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <syslog.h>
#include "mainpage.h"
#include "shared.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
  void signalToServerSomething(const QString &mesaj);


private slots:
    void on_pushButton_Start_clicked();

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
