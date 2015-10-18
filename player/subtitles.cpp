#include "subtitles.h"
#include "mainwindow.h"
#include "videoplayer.h"
#include "timeconteiner.h"
#include <QKeyEvent>
#include <QApplication>
#include <QMessageBox>

Subtitles::Subtitles(QWidget* parent, MainWindow *wind):
    QTextBrowser(parent)
{
    this->wind=wind;
    trans=new RequestManager;
}
void Subtitles::mouseReleaseEvent(QMouseEvent *){
    QString translateRequest=textCursor().selectedText();
    if(translateRequest.trimmed()=="") return;
    wind->setCursor(Qt::WaitCursor);
    this->viewport()->setCursor(Qt::WaitCursor);
    connect(trans,SIGNAL(finish(QMap<QString,QString>,Action)),wind,SLOT(dataReceive(QMap<QString,QString>,Action)));
    connect(trans,SIGNAL(error(QString)),wind,SLOT(errorWithRequest(QString)));
    trans->translate(translateRequest);
}
void Subtitles::keyPressEvent(QKeyEvent *ke){
        QApplication::sendEvent(wind->getPlayer(),ke);
}
void Subtitles::enterEvent(QEvent *){
    if(wind->getPlayer()->isPlaying())
        wind->on_play_clicked();
}

void Subtitles::leaveEvent(QEvent *){
    if(wind->getPlayer()->isPaused())
        wind->on_play_clicked();
    wind->setFocus();
}
QMap<TimeConteiner, QString> * Subtitles::loadFile(QString &filename){
    QTextStream in;
    in.setDevice(new QFile(filename));
    if(!in.device()->open(QIODevice::ReadOnly|QIODevice::Text)) return NULL;

    QMap<TimeConteiner,QString>* data=new QMap<TimeConteiner,QString>();
    in.setCodec("utf-8");
    while(!in.atEnd()){
        QString tStart,arow,tEnd,replic,scene;
        scene=in.readLine();// read a scene number
        in>>tStart>>arow>>tEnd;// read a duration of
        if(arow.trimmed()!="-->"||scene.trimmed().toInt()==0){ // if don't find an arrow or do not succesful convert a scen no
            QMessageBox mb(this);
            mb.setText(tr("Неверный формат файла"));
            mb.exec();
            return NULL;
        }
        QTime start,end;
        start=start.fromString(tStart,"hh:mm:ss,zzz");
        end=end.fromString(tEnd,"hh:mm:ss,zzz");
        replic=in.readLine();
        QString str1="";
        while((str1=(str1=in.readLine()).trimmed())!="")
            replic+="\n"+str1;
        TimeConteiner  tc=TimeConteiner(start,end);
        data->insert(tc,replic);
      }
    return data;
}
Subtitles::~Subtitles(){

}
