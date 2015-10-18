#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextDocumentFragment>
#include <QLayout>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include "timeconteiner.h"
#include "subtitles.h"
#include "videoplayer.h"
#include "registration.h"

#define MESSAGE(str) QMessageBox mb;mb.setText(str);mb.exec();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sub=NULL;
    videoPlayer=new VideoPlayer(this,this);
    subtitles=new Subtitles(this,this);
    subtitles->setReadOnly(true);
    subtitles->hide();
    ui->autoris->setText("Вы не авторизировались <a href=\"aut\">Авторизироваться</a>");
    ui->logOut->setText("");
    ui->closePan->setText("<a href=\"close\"><font color=\"red\">X</font></a>");
    setLayouts();
    connect(ui->action,SIGNAL(triggered()),this,SLOT(on_open_action_click()));
    connect(ui->action_2,SIGNAL(triggered()),this,SLOT(on_exit_action()));
    connect(ui->action_3,SIGNAL(triggered()),this,SLOT(on_open_subs_click()));
    connect(ui->action_5,SIGNAL(triggered()),this,SLOT(registretion_clicked()));
    connect(ui->play,SIGNAL(clicked()),this,SLOT(on_play_clicked()));
    connect(videoPlayer,SIGNAL(on_screen_state_change(bool)),this,SLOT(on_videoplayer_change_state(bool)));
    ui->pushButton->setToolTip(tr("Добавить слово в мой словарь"));

    if(QApplication::arguments().size()>1)
      videoPlayer->play(QApplication::arguments().at(1).trimmed());

    ac=new RequestManager();
    connect(ac,SIGNAL(finish(QMap<QString,QString>,Action)),this,SLOT(dataReceive(QMap<QString,QString>,Action)));
    QTimer::singleShot(10,this,SLOT(lazyInit()));
}

MainWindow::~MainWindow()
{
    delete videoPlayer;
    delete ui;
}
void MainWindow::on_time(){
    QTime time=QTime(0,0,0,0);
    time=time.addMSecs(videoPlayer->currentTime());
    if(sub!=NULL){
        QString replic("");
        TimeConteiner tc(time,time);
        replic=sub->value(tc,replic).trimmed();
        if(replic!=subtitles->toPlainText().trimmed()){
            subtitles->setText(replic);
        }
    }
    ui->label->setText(time.toString("hh:mm:ss"));
}
void MainWindow::setLayouts(){
    QVBoxLayout * mainLayout=new QVBoxLayout(),*controlMainLayout=new QVBoxLayout();
    QHBoxLayout * soundLayout=new QHBoxLayout(),* seekSliderLayout=new QHBoxLayout();
    QHBoxLayout * withTranslate=new QHBoxLayout;
    QHBoxLayout * autorisBoxLayout=new QHBoxLayout;
    QVBoxLayout * translateBoxLayout=new QVBoxLayout();
    showMaximized();
    withTranslate->addWidget(videoPlayer);
    withTranslate->addWidget(ui->translateBox);
    mainLayout->addWidget(ui->autorisBox);
    mainLayout->addLayout(withTranslate);
    mainLayout->addWidget(ui->controlPanelBox);
    withTranslate->setMargin(0);
    mainLayout->setMargin(0);

    seekSliderLayout->addWidget(ui->seekSlider);
    seekSliderLayout->addWidget(ui->label);
    soundLayout->addWidget(ui->buttonsPanel,width()*3/4);
    soundLayout->addWidget(ui->volumeSlider);   

    ui->centralWidget->setLayout(mainLayout);
    controlMainLayout->addLayout(seekSliderLayout);
    controlMainLayout->addLayout(soundLayout);

    ui->controlPanelBox->setLayout(controlMainLayout);

    autorisBoxLayout->addWidget(ui->autoris,4);
    autorisBoxLayout->addWidget(ui->logOut);
    autorisBoxLayout->addWidget(ui->closePan);
    ui->autorisBox->setLayout(autorisBoxLayout);
    translateBoxLayout->addWidget(ui->translations);
    translateBoxLayout->addWidget(ui->pushButton);
    translateBoxLayout->addWidget(ui->label_2);
    translateBoxLayout->addWidget(ui->pushButton_2);
    ui->translateBox->setLayout(translateBoxLayout);
    ui->translateBox->setVisible(false);
}
void MainWindow::on_open_action_click(){
    QString filter;
    filter=tr("Видео файлы(*.avi *.mkv *.3gpp *.divx *.flv *.mp4 *.mpeg);;Аудио файлы(*.mp3 *.mpeg *.wav)");
    QString source=QFileDialog::getOpenFileName(this,tr("Открыть видео файл"),tr(""),filter);
    if(source.isEmpty()) return;
    on_finish_player();
    videoPlayer->play(source.trimmed());
}
void MainWindow::on_open_subs_click(){
    QString source=QFileDialog::getOpenFileName(0,tr("Загрузить субтитры"),tr(""),tr("Субтитры(*.srt *.txt)"));
    source=source.trimmed();
    if(source=="") return;
    if(sub!=NULL){
        delete sub;
        sub=NULL;
    }
    sub=subtitles->loadFile(source);
    if(sub!=NULL&&!subtitles->isVisible()) subtitles->show();
    else subtitles->hide();
}
void MainWindow::on_finish_player(){
    if(sub!=NULL){
        delete sub;
        sub=NULL;
    }
    if(subtitles->isVisible()) subtitles->hide();
    ui->label->setText(tr("00:00:00"));
    ui->action_3->setEnabled(false);
    ui->seekSlider->setMaximum(0);
    changeIcons(Play,false);
}
void MainWindow::set_translate(QString en, QString ru){
    ui->translations->setHtml("<font color=\"purple\" size=5>"+en+"</font><hr align=\"center\" width=\"500\" size=\"2\" color=\"#ff0000\" /><font color=\"olive\" size=6>"+ru+"</font> <p><p><p>Переведено сервисом <a href=\"http://translate.yandex.ua\">«Яндекс.Перевод»</a>");
    ui->translateBox->setVisible(true);
    setCursor(Qt::ArrowCursor);
    subtitles->viewport()->setCursor(Qt::ArrowCursor);    
}

VideoPlayer* MainWindow::getPlayer() const{
    return videoPlayer;
}
QFrame* MainWindow::getPanel() const{
    return ui->controlPanelBox;
}
QPushButton* MainWindow::getPlayButton() const{
    return ui->play;
}
void MainWindow::on_exit_action(){
    close();
}

void MainWindow::on_videoplayer_change_state(bool inFScreen){
    setUpdatesEnabled(false);
    if(inFScreen) showMaximized();
    else showFullScreen();

    changeIcons(Resize,inFScreen);
    ui->controlPanelBox->setVisible(inFScreen);
    ui->menuBar->setVisible(inFScreen);
    if(ui->closePan->text()!=""||ui->logOut->text()==""){
        ui->autorisBox->setVisible(inFScreen);
        ui->closePan->setText("<a href=\"close\"><font color=\"red\">X</font></a>");
    }
    setUpdatesEnabled(true);
}
void MainWindow::changeIcons(MainWindow::Buttons but,bool itIs){
    switch(but){
    case Play: if(itIs) ui->play->setIcon(QIcon("src\\pause.png"));
                else ui->play->setIcon(QIcon("src\\play.png"));
        break;
    case Resize: if(itIs) ui->full->setIcon(QIcon("src\\full_screen.png"));
                   else ui->full->setIcon(QIcon("src\\partial_screen.png"));
    }
}

void MainWindow::on_stop_clicked()
{
    videoPlayer->stop();
    on_finish_player();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->translateBox->setVisible(false);
    ui->label_2->setText("");
}
void MainWindow::on_full_clicked(bool)
{
    on_videoplayer_change_state(isFullScreen());
}
void MainWindow::on_play_clicked()
{
    if(videoPlayer->isPlaying()){
        videoPlayer->pause();
        changeIcons(Play,false);
    }
    else if(videoPlayer->isPaused()){
        videoPlayer->play();
        changeIcons(Play,true);
    }
}

void MainWindow::on_seekSlider_sliderMoved(int value)
{
       videoPlayer->seek((value*videoPlayer->duration())/100);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    videoPlayer->volume(position);
}

void MainWindow::on_back_clicked()
{
    QApplication::postEvent(videoPlayer,new QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier));
}

void MainWindow::on_forward_clicked()
{
    QApplication::postEvent(videoPlayer,new QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier));
}
void MainWindow::registretion_clicked(){
    videoPlayer->pause();

    Registration* reg=new Registration(this);
    connect(reg,SIGNAL(autorisOk(QString,QString)),this,SLOT(autorisation(QString,QString)));
    connect(reg,SIGNAL(finished(int)),this,SLOT(on_autorisation_close(int)));
    reg->exec();
}
void MainWindow::autorisation(QString, QString name){
    QString firstLater=name.at(0);
    ui->autoris->setText("Вы вошли как <font color=\"red\" size=5>"+firstLater+"</font>"+name.remove(0,1)+"");
    ui->logOut->setText("<a href=\"ex\">Выход</a>");
    ui->closePan->setText("<a href=\"close\"><font color=\"red\">X</font></a>");
    ui->autorisBox->setVisible(true);
}

void MainWindow::on_logOut_linkActivated(const QString &)
{
    ac->logOut();
    ui->autoris->setText("Вы не авторизировались <a href=\"aut\">Авторизироваться</a>");
    ui->logOut->setText("");
}

void MainWindow::on_pushButton_clicked()
{
    connect(ac,SIGNAL(error(QString)),this,SLOT(errorWithRequest(QString)));
    ac->write(ru,en);
    ui->label_2->setText("Добавлено!");

}

void MainWindow::on_translations_textChanged()
{
    ui->label_2->setText("");
}
void MainWindow::on_autorisation_close(int){
    on_play_clicked();
}

void MainWindow::on_autoris_linkActivated(const QString&)
{
    registretion_clicked();
}

void MainWindow::on_closePan_linkActivated(const QString&)
{
    ui->autorisBox->setVisible(false);
    ui->closePan->setText("");
}
void MainWindow::dataReceive(QMap<QString, QString> data, Action acc){

    if(acc==LOG_IN_VIA_HASH)
        autorisation(data["hash"],data["name"]);
    if(acc==TRANSLATION){
        en=data["original"];
        ru=data["translate"];
        set_translate(en,ru);
    }

}
void MainWindow::errorWithRequest(QString msg){
    MESSAGE(msg);
    setCursor(Qt::ArrowCursor);
    subtitles->viewport()->setCursor(Qt::ArrowCursor);
    if(ui->label_2->text()=="Добавлено!")
          ui->label_2->setText("Ошибка при добавлении");
}
void MainWindow::lazyInit(){
    ac->viaHash();
}

