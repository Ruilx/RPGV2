#ifndef RPGSOUNDHELPER_H
#define RPGSOUNDHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgSound.h>

class RpgSoundHelper : public QObject
{
	Q_OBJECT

	RpgSound *sound = nullptr;
public:
	explicit RpgSoundHelper(RpgSound *rpgSound, QObject *parent = nullptr) : QObject(parent){
		if(rpgSound == nullptr){
			qDebug() << CodePath() << "RpgSound is nullptr.";
			return;
		}
		this->sound = rpgSound;
	}

	Q_INVOKABLE void addSound(const QString &soundName, const QString &fileName){
		if(this->sound == nullptr){
			qDebug() << CodePath() << "RpgSound is nullptr.";
			return;
		}
		this->sound->addSound(soundName, fileName);
	}

	Q_INVOKABLE void removeSound(const QString &soundName){
		if(this->sound == nullptr){
			qDebug() << CodePath() << "RpgSound is nullptr.";
			return;
		}
		this->sound->removeSound(soundName);
	}

	Q_INVOKABLE void clearSound(){
		if(this->sound == nullptr){
			qDebug() << CodePath() << "RpgSound is nullptr.";
			return;
		}
		this->sound->clearSound();
	}

	Q_INVOKABLE void play(const QString &soundName, qreal volume = 0.7f, int times = 1){
		if(this->sound == nullptr){
			qDebug() << CodePath() << "RpgSound is nullptr.";
			return;
		}
		this->sound->play(soundName, volume, times);
	}

	Q_INVOKABLE void stop(const QString &soundName){
		if(this->sound == nullptr){
			qDebug() << CodePath() << "RpgSound is nullptr.";
			return;
		}
		this->sound->stop(soundName);
	}

signals:

public slots:
};

#endif // RPGSOUNDHELPER_H
