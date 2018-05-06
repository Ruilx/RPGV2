#ifndef RPGSOUND_H
#define RPGSOUND_H

#include <QtCore>
#include <QSoundEffect>
#include <RPG/Global.h>

class RpgSound : public QObject
{
	Q_OBJECT
	static RpgSound *_instance;

	QHash<QString, QSoundEffect*> soundMap;
public:
	static RpgSound *instance(){
		if(_instance == nullptr){
			_instance = new RpgSound(nullptr);
		}
		return _instance;
	}

	explicit RpgSound(QObject *parent = nullptr) : QObject(parent){
		this->soundMap.clear();
	}

	void addSound(const QString &soundName, const QString &fileName){
		if(!QFile::exists(fileName)){
			qDebug() << CodePath() << "Sound file:'" << fileName << "' is not exist.";
		}
		QSoundEffect *sound = new QSoundEffect(this);
		sound->setSource(QUrl(fileName));
		this->soundMap.insert(soundName, sound);
	}

	void removeSound(const QString &soundName){
		QSoundEffect *sound = this->soundMap.take(soundName);
		if(sound != nullptr){
			if(sound->isPlaying()){
				sound->stop();
			}
			sound->deleteLater();
		}
	}

	void clearSound(){
		for(QSoundEffect *sound: this->soundMap){
			if(sound != nullptr){
				if(sound->isPlaying()){
					sound->stop();
				}
				sound->deleteLater();
			}
		}
		this->soundMap.clear();
	}
signals:
	void started(const QString &soundName);
	void stopped(const QString &soundName);
public slots:
	void play(const QString &soundName, int times = 1){
		QSoundEffect *sound = this->soundMap.value(soundName, nullptr);
		if(sound == nullptr){
			qDebug() << CodePath() << "Sound name is invalid or not exist.";
			return;
		}else{
			if(sound->isPlaying()){
				sound->stop();
			}
			sound->setLoopCount(times);
			sound->play();
			emit this->started(soundName);
			qDebug() << CodePath() << "Sound effect: '" << sound->source().url(QUrl::PreferLocalFile) << "'";
		}
	}

	void stop(const QString &soundName){
		QSoundEffect *sound = this->soundMap.value(soundName, nullptr);
		if(sound == nullptr){
			qDebug() << CodePath() << "Sound name is invalid or not exist.";
			return;
		}else{
			if(sound->isPlaying()){
				sound->stop();
				emit this->stopped(soundName);
			}
		}
	}
};

#endif // RPGSOUND_H
