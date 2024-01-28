#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "shared.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Start_clicked()
{
    QMessageBox::information(this,"Start","Application Started!");
    this->hide();
    MainPage *mainpage = new MainPage();
    mainpage->show();

}

