#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    count=0;
    ui->setupUi(this);
    connect(ui->action,SIGNAL(triggered(bool)),this,SLOT(on_autorisation_click(bool)));
    manager=new RequestManager;
    connect(manager,SIGNAL(finish(QMap<QString,QString>,Action)),this,SLOT(on_request(QMap<QString,QString>,Action)));
    connect(manager,SIGNAL(error(QString)),this,SLOT(smError(QString)));
    manager->viaHash();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ru.trimmed().toUpper()==ui->translation->text().trimmed().toUpper()){
        QMessageBox mb;
        mb.setText("Правильно!!!");
        mb.exec();
        on_pushButton_2_clicked();
    }
    else{
        QMessageBox mb;
        mb.setText("Не правильно.");
        mb.exec();
    }


}
void MainWindow::on_autorisation_click(bool){
    Registration* reg=new Registration(this);
    connect(reg,SIGNAL(autorisOk(QString,QString)),this,SLOT(setName(QString,QString)));
    reg->exec();
}
void MainWindow::on_request(QMap<QString,QString> data,Action ac){
   if(ac==LOG_IN_VIA_HASH)
       setName(data["hash"],data["name"]);
   if(ac==READ){
       ui->forTranslate->setText(data["en"]);
       ru=data["ru"];
   }
   if(ac==INFO)
       count=data["info"].toInt();
}
void MainWindow::smError(QString msg){
    QMessageBox mb;
    mb.setText(msg);
    mb.exec();
    if(msg.contains("log in or register"))
        on_autorisation_click(true);
}
void MainWindow::setName(QString, QString name){
    ui->userName->setText("Вы вошли как "+name);
    manager->info();
    ui->pushButton_2->setText("Старт");
}

void MainWindow::on_pushButton_2_clicked()
{
    if(count==0){
        QMessageBox mb;
        mb.setText("No more words.");
        mb.exec();
        return;
    }
    ui->pushButton_2->setText("Следующее");
    static unsigned int index=count;
    if(index>0)
        manager->read(index--);
    else
        index=count;
}
