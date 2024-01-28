#include "system.h"
#include "ui_system.h"

System::System(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::System)
{
    ui->setupUi(this);
    Worker *worker12 = new Worker();
    connect(this,&System::emitSemnalToServer,worker12,&Worker::handleRequesFromSystem,Qt::QueuedConnection);
    connect(worker12,&Worker::sendResponseToSystem,this,&System::receivedMessageFromWorker);

    setUpComboBoxOptions(SharedData::getUsername());

}

void System::setUpComboBoxOptions(const QString &username) // Aici setam adresele IP pentru fiecare utilziator. Pentru ca un user nu are voie sa vada agentii altor utilizatori, doar pe ale lui
{
    QFile file("ip_list.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open user file");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ");

        if (parts.size() == 2 && parts[0] == username) {
            ui->comboBox_IP->addItem(parts[1]);
        }
    }
    file.close();

    if(ui->comboBox_IP->count() == 1){
        QMessageBox::information(this, "System", "You need to add an IP adress.Go to the Add Agent section."); // Aici verificam daca user-ul are adaugata macar o adresa IP/
        this->hide();                                                                                          // Daca nu are niciuna, nu are voie sa intre.
        MainPage *mainpage = new MainPage();
        mainpage->show();
    }

}

System::~System()
{
    delete ui;
}

void System::receivedMessageFromWorker(const QString &mesaj)
{
    ui->textBrowser_logs->clear();
    ui->textBrowser_logs->setText(mesaj);

}

void System::setamIP()
{
    qDebug() << "suntem in setam ip";
}

void System::on_pushButton_clicked()
{
    if(ui->comboBox_IP->currentIndex() == 0){
        QMessageBox::information(this, "System", "You need to select an IP adress.");
    }
    else{
        QString ip_adress = ui->comboBox_IP->currentText();
        qDebug() <<ip_adress;
        QString type_of = ui->comboBox_type_of_log->currentText();
        qDebug() << type_of;
        QString quantity = ui->comboBox_quantity->currentText();
        qDebug() << quantity;
        QString period = ui->comboBox_time->currentText();
        qDebug() << period;
        ui->comboBox_type_of_log->setCurrentIndex(0);
        ui->comboBox_quantity->setCurrentIndex(0);
        ui->comboBox_time->setCurrentIndex(0);
        emit emitSemnalToServer(ip_adress,type_of,quantity,period);
    }
}


void System::on_pushButton_2_clicked()
{
    this->hide();
    MainPage *mainpage = new MainPage();
    mainpage->show();
}

