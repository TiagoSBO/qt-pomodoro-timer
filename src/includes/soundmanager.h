#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMap>
#include <QUrl>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    explicit SoundManager(QObject *parent = nullptr);
    void playSound(int index);
    void setVolume(int volume);
    void setSoundEnabled(bool enabled);

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QMap<int, QUrl> soundMap;
    void loadSounds();
    bool soundEnabled = true;

};

#endif
