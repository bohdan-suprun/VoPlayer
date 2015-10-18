#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QTimer>
#include <QKeyEvent>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlayer>

class MainWindow;

class VideoPlayer: public QVideoWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget*, MainWindow *wind);
    virtual ~VideoPlayer();
    int currentTime();
    bool isPaused() const;
    bool isPlaying() const;
    void play(QString);
    void play();
    void pause();
    void stop();
    void seek(qint64);
    qint64 duration()const;
signals:
    void on_screen_state_change(bool inFScreen);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual bool eventFilter(QObject *, QEvent *);
    virtual void resizeEvent(QResizeEvent *);
public slots:
    void volume(int);
private slots:
    void setControlPanelInvisible();
    void progress(qint64);
    void error(QMediaPlayer::Error);

private:
   QMediaPlayer * player;
   QTimer * timer;
   MainWindow* parent;
   bool enter;
};

#endif // VIDEOPLAYER_H
