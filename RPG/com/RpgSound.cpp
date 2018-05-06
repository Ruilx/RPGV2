#include "RpgSound.h"

RpgSound *RpgSound::_instance = nullptr;

void RpgSound::addSound(const QString &soundName, const QString &fileName){
	if(!QFile::exists(fileName)){
		qDebug() << CodePath() << "Sound file:'" << fileName << "' is not exist.";
	}
	QSound *sound = new QSound(fileName, this);
	this->soundMap.insert(soundName, sound);
}

void RpgSound::removeSound(const QString &soundName){
	QSound *sound = this->soundMap.take(soundName);
	if(sound != nullptr){
		if(!sound->isFinished()){
			sound->stop();
		}
		sound->deleteLater();
	}
}

void RpgSound::clearSound(){
	for(QSound *sound: this->soundMap){
		if(sound != nullptr){
			if(!sound->isFinished()){
				sound->stop();
			}
			sound->deleteLater();
		}
	}
	this->soundMap.clear();
}

void RpgSound::play(const QString &soundName, /*qreal volume,*/ int times){
	QSound *sound = this->soundMap.value(soundName, nullptr);
	if(sound == nullptr){
		qDebug() << CodePath() << "Sound name is invalid or not exist.";
		return;
	}else{
//		if(sound->status() == QSoundEffect::Error){
//			qDebug() << CodePath() << "Sound status error.";
//			return;
//		}
		if(!sound->isFinished()){
			sound->stop();
		}
		sound->setLoops(times);
//		sound->setVolume(volume);
		sound->play();
		emit this->started(soundName);
		qDebug() << CodePath() << "Sound effect: '" << sound->fileName() << "'";
	}
}

void RpgSound::stop(const QString &soundName){
	QSound *sound = this->soundMap.value(soundName, nullptr);
	if(sound == nullptr){
		qDebug() << CodePath() << "Sound name is invalid or not exist.";
		return;
	}else{
		if(!sound->isFinished()){
			sound->stop();
			emit this->stopped(soundName);
		}
	}
}
