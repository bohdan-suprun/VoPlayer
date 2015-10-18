#include "videoplayer.h"
#include <QMultimedia>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subtitles.h"

#define MESSAGE(str) QMessageBox mb;mb.setText(str);mb.exec();

#define PAN_VISIBLE_TIME 3000
#define SEEK_STEP 5000

VideoPlayer::VideoPlayer(QWidget *parent2, MainWindow * wind)
    :QVideoWidget(parent2)
{
    this->parent=wind;
    player=new QMediaPlayer;
    enter=0;
    timer=new QTimer();
    this->parent->getPanel()->installEventFilter(this);
    this->parent->installEventFilter(this);
    this->parent->ui->volumeSlider->installEventFilter(this);
    this->parent->ui->seekSlider->installEventFilter(this);
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(progress(qint64)));
    connect(player,SIGNAL(volumeChanged(int)),this,SLOT(volume(int)));
    player->setVolume(this->parent->ui->volumeSlider->value());
    player->setVideoOutput(this);
    connect(player,SIGNAL(error(QMediaPlayer::Error)),this,SLOT(error(QMediaPlayer::Error)));
    setMouseTracking(true);
}
void VideoPlayer::mouseDoubleClickEvent(QMouseEvent *){
     emit on_screen_state_change(parent->isFullScreen());
}
void VideoPlayer::mousePressEvent(QMouseEvent *){

    if(parent->isFullScreen()&&!timer->isActive()){
        parent->getPanel()->setVisible(true);
        parent->getPanel()->setFocus();
        timer->start(PAN_VISIBLE_TIME);
        connect(timer,SIGNAL(timeout()),this,SLOT(setControlPanelInvisible()));
    }
}
void VideoPlayer::setControlPanelInvisible(){
    if(parent->isFullScreen()&&!enter) parent->getPanel()->setVisible(false);
    else parent->getPanel()->setVisible(true);
    parent->setFocus();
    timer->stop();
   
}
void VideoPlayer::keyPressEvent(QKeyEvent *ke){
    if(ke->key()==Qt::Key_Escape)
        if(parent->isFullScreen()) emit on_screen_state_change(true);
    if(ke->key()==Qt::Key_Space) parent->on_play_clicked();
    if(ke->key()==Qt::Key_Left) player->setPosition(currentTime()-SEEK_STEP);
    if(ke->key()==Qt::Key_Right)player->setPosition(currentTime()+SEEK_STEP);
}
bool VideoPlayer::eventFilter(QObject *ob, QEvent *ev){
    switch(ev->type()){
    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent*>(ev));
        break;
    case QEvent::Enter:
        if(ob==parent->getPanel()){
            enter=1;
        }
        break;
    case QEvent::Leave:
        if(ob==parent->getPanel()){
            if(enter) mousePressEvent((QMouseEvent*)ev);
            enter=0;
        }
        break;
    default:
        ob->event(ev);
    }
        return true;
}
void VideoPlayer::resizeEvent(QResizeEvent *ev){
    int x=width()/2-155;
    int y=height()-75;
    parent->subtitles->setGeometry(x,y,300,70);
    QVideoWidget::resizeEvent(ev);
}
bool VideoPlayer::isPlaying() const{
    return player->state()==1;
}

bool VideoPlayer::isPaused() const{
    return player->state()==2;
}

int VideoPlayer::currentTime(){
    return player->position();
}
void VideoPlayer::play(QString path){

    player->setMedia(QUrl::fromLocalFile(path));
    parent->changeIcons(MainWindow::Play,true);
    parent->ui->action_3->setEnabled(true);
    parent->ui->seekSlider->setMaximum(100);
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),parent,SLOT(on_time()));
    timer->start(100);
    player->play();
}
void VideoPlayer::play(){
    player->play();
}
void VideoPlayer::pause(){
    player->pause();
}
void VideoPlayer::stop(){
    player->stop();
}
void VideoPlayer::progress(qint64){
    double pos=(double)player->position()/player->duration();
    if(!parent->ui->seekSlider->isSliderDown())
        parent->ui->seekSlider->setValue(pos*100);
}
void VideoPlayer::volume(int val){
    player->setVolume(val);
}
void VideoPlayer::seek(qint64 pos){
    player->setPosition(pos);
}
qint64 VideoPlayer::duration() const{
    return player->duration();
}
VideoPlayer::~VideoPlayer(){
    delete player;
}
void VideoPlayer::error(QMediaPlayer::Error er){
   if(er==0||er==2) return;
    MESSAGE("Произошла ошибка при воспроизведении.")
    parent->on_stop_clicked();
    qDebug()<<er;
}
