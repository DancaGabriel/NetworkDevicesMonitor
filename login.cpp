#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(ui->lineEdit_username, &QLineEdit::returnPressed, this, &Login::on_usernameReturnPressed);
    Worker *worker = new Worker();
    connect(this,&Login::signalToServerSomething,worker,&Worker::primimLogin);
    connect(this,&Login::trimitLaServer,worker,&Worker::primimUsername);

    //setWindowFlag(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground,true);
}

Login::~Login()
{
    delete ui;
}

void Login::handleMessage(const QString& message) {
    // qDebug() << "sunt;

}

void Login::on_pushButton_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open user file");
        return;
    }

    QTextStream in(&file);
    bool loginSuccessful = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ");

        if (parts.size() == 2 && parts[0] == username && parts[1] == password) {
            loginSuccessful = true;
            break;
        }
    }

    file.close();

    if (loginSuccessful) {
        emit signalToServerSomething("Ana are mere");
        qDebug() << "Username-ul este: " << username;
        emit trimitLaServer(username);
        SharedData::setUsername(username);
        QMessageBox::information(this, "Login", "Loggin succesful, Hello Boss!");
        MainWindow *mainwindow = new MainWindow();
        this->hide();
        mainwindow->show();

    } else {
        QMessageBox::information(this, "Login", "Credentials wrong, try again or leave!");
    }
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}


void Login::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Login","Are you sure you want to exit the application?",QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QApplication::quit();
    }
}

// void Login::on_pushButton_2_clicked()
// {

// }

void Login::on_usernameReturnPressed()
{
    ui->lineEdit_password->setFocus();

}
