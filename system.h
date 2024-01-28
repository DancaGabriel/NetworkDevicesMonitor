#ifndef SYSTEM_H
#define SYSTEM_H

#include <QMainWindow>
#include <QDebug>
#include "worker.h"

namespace Ui {
class System;
}

class System : public QMainWindow
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = nullptr);
    void setUpComboBoxOptions(const QString& username);
    ~System();

public slots:
   void receivedMessageFromWorker(const QString &mesaj);
   void setamIP();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void emitSemnalToServer(QString ip,QString type_of,QString quantity,QString period);

private:
    Ui::System *ui;
};

#endif // SYSTEM_H
