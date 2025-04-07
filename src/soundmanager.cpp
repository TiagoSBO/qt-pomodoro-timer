#include "soundmanager.h"
#include <QDebug>

SoundManager::SoundManager(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    loadSounds();
}

void SoundManager::loadSounds()
{
    soundMap[0] = QUrl("qrc:/sounds/assets/sounds/alarm_cuckoo1.wav");
    soundMap[1] = QUrl("qrc:/sounds/assets/sounds/alarm_cuckoo2.wav");
    soundMap[2] = QUrl("qrc:/sounds/assets/sounds/digitalalarm_clock1.wav");
    soundMap[3] = QUrl("qrc:/sounds/assets/sounds/digitalalarm_clock2.wav");
    soundMap[4] = QUrl("qrc:/sounds/assets/sounds/digitalalarm_clock3.wav");
}

void SoundManager::playSound(int index)
{
    if (soundMap.contains(index)) {
        qDebug() << "Tocando som do índice:" << index << "URL:" << soundMap[index];
        player->stop();                // Para qualquer som que esteja tocando
        player->setSource(soundMap[index]); // Define o novo som
        player->play();                 // Toca o som
    } else {
        qDebug() << "Erro: índice inválido para som.";
    }
}

void SoundManager::setVolume(int volume)
{
    if (audioOutput) {
        float normalizedVolume = static_cast<float>(volume) / 100.0;
        audioOutput->setVolume(normalizedVolume);
        qDebug() << "Volume atualizado para:" << normalizedVolume;
    }
}
