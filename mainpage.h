#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>
#include "system.h"
#include "helpwindow.h"
#include "addagent.h"
#include "worker.h"

namespace Ui {
class MainPage;
}

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    QString username_mainpage;
    ~MainPage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public slots:
    void metricsDashboard(const QString& measaj);
private:
    Ui::MainPage *ui;
};

#endif // MAINPAGE_H
