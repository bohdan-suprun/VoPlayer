#include "registration.h"
#include "ui_registration.h"
#include <QDebug>
#include <QMessageBox>

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    ui->reg->setVisible(false);
    req=new RequestManager();

    connect(req,SIGNAL(error(QString)),this,SLOT(displayError(QString)));
    connect(req,SIGNAL(finish(QMap<QString,QString>,Action)),this,SLOT(data(QMap<QString,QString>,Action)));
}

Registration::~Registration()
{
    delete ui;
}
void Registration::on_label_8_linkActivated(const QString&)
{
    ui->signIn->setVisible(false);
    ui->reg->setVisible(true);
    ui->name->setText("");
    ui->mail->setText("");
    ui->pass->setText("");
    ui->passPrompt->setText("");
}
void Registration::on_signInBtn_clicked()
{
    ui->signIn->setEnabled(false);
    ui->signIn->repaint();
    req->logIn(ui->mailSign->text(),ui->passSign->text());
}

void Registration::on_regBtn_clicked()
{
    try{
        if(ui->name->text().isEmpty())
           throw QString("Вы не указали Ваше имя.");
        if(ui->mail->text().trimmed().indexOf("@")==-1||ui->mail->text().trimmed().indexOf("@")==ui->mail->text().trimmed().length()-1||ui->mail->text().trimmed().indexOf("@")==0)
            throw QString("Електронная почта указана неверно.");
        if(ui->pass->text().trimmed()!=ui->passPrompt->text().trimmed())
            throw QString("Пароли не совпадают.");
        if(ui->pass->text().trimmed().isEmpty())
            throw QString("Пароль не должен быть пустым.");
        if(ui->pass->text().length()<5)
            throw QString("Пароль должен быть не короче 5 символов");
        for(int i=0;i<ui->pass->text().length();i++){
            QChar symb=ui->pass->text().at(i); //125
            if(symb<30||symb>126) // it isn't between ' '(space) and '~'
                throw QString("Пароль может содержать только латиницу,цифры и специальные символы (@!^*...).");
        }
        ui->reg->setEnabled(false);
        ui->reg->repaint();
        req->registration(ui->name->text(),ui->mail->text(),ui->pass->text());

    }
    catch(QString ex){
        displayError(ex);
    }
}
void Registration::displayError(QString error){
    QMessageBox mb;
    mb.setIcon(QMessageBox::Information);
    mb.setText(error);
    mb.exec();
    ui->reg->setEnabled(true);
    ui->signIn->setEnabled(true);
}
void Registration::on_pushButton_clicked()
{
    ui->signIn->setVisible(true);
    ui->reg->setVisible(false);
}
void Registration::data(QMap<QString, QString> data, Action ac){
    if(ac==LOG_IN){
        ui->signIn->setEnabled(true);
        emit autorisOk(data["hash"],data["name"]);
        close();
    }
    if(ac==REGISTRATION){
        ui->reg->setEnabled(true);
        ui->signIn->setVisible(true);
        ui->mailSign->setText(ui->mail->text());
        ui->passSign->setText("");
        ui->reg->setVisible(false);
    }

}
