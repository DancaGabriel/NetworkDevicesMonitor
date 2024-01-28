#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    Worker *worker999 = new Worker();
    connect(worker999,&Worker::sendSecurityIncidents,this,&MainPage::metricsDashboard,Qt::QueuedConnection);



}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::on_pushButton_clicked()
{
    System *system = new System();
    this->hide();
    system->show();

}


void MainPage::on_pushButton_2_clicked()
{
    AddAgent *agent =  new AddAgent();
    this->hide();
    agent->show();

}


void MainPage::on_pushButton_3_clicked()
{
    HelpWindow *help = new HelpWindow();
    this->hide();
    help->show();

}

void MainPage::metricsDashboard(const QString &mesaj)
{
    qDebug() << "Suntem in metrics";
    if(mesaj.contains("entries")){
       ui->label_5->setText("0");
    }
    else{
        ui->label_5->setText("You have security incidents. Go look at the logs!");;
    }
}

