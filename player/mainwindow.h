#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QPushButton>
#include <QGroupBox>
#include "requestmanager.h"

namespace Ui {
class MainWindow;
}
class Subtitles;
class VideoPlayer;
class TimeConteiner;
class Registration;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void setUser(QString);
    friend class VideoPlayer;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    VideoPlayer* getPlayer() const;
    QPushButton* getPlayButton() const;
    QFrame *getPanel() const;
public slots:
    void on_play_clicked();
private slots:
    void autorisation(QString,QString);
    void on_time();
    void on_open_action_click();
    void on_exit_action();
    void on_open_subs_click();
    void on_finish_player();
    void set_translate(QString,QString);
    void on_videoplayer_change_state(bool);
    void on_stop_clicked();
    void on_pushButton_2_clicked();
    void on_full_clicked(bool checked);
    void on_seekSlider_sliderMoved(int position);
    void on_volumeSlider_sliderMoved(int position);
    void on_back_clicked();
    void on_forward_clicked();
    void registretion_clicked();
    void on_logOut_linkActivated(const QString &link);
    void on_pushButton_clicked();
    void on_translations_textChanged();
    void on_autorisation_close(int);
    void on_autoris_linkActivated(const QString &link);
    void on_closePan_linkActivated(const QString &link);
    void dataReceive(QMap<QString,QString>,Action);
    void errorWithRequest(QString);
    void lazyInit();
private:
    RequestManager * ac;
    QTimer * t;
    QString ru,en;
    enum Buttons{Play,Resize};
    void changeIcons(Buttons but,bool flag);
    VideoPlayer * videoPlayer;
    QMap<TimeConteiner,QString>* sub;
    Subtitles * subtitles;
    Ui::MainWindow *ui;
    void setLayouts();
};

#endif // MAINWINDOW_H
