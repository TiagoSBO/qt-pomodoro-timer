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

private:
    QMediaPlayer *player;      // Gerenciador de áudio
    QAudioOutput *audioOutput; // Saída de áudio
    QMap<int, QUrl> soundMap;  // Mapeamento de sons
    void loadSounds();
};

#endif // SOUNDMANAGER_H
