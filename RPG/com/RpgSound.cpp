#include "RpgSound.h"

RpgSound *RpgSound::_instance = nullptr;

void RpgSound::addSound(const QString &soundName, const QString &fileName){
	if(!QFile::exists(fileName)){
		qDebug() << CodePath() << "Sound file:'" << fileName << "' is not exist.";
	}
	QSoundEffect *sound = new QSoundEffect(this);
	sound->setSource(QUrl(fileName));
	this->soundMap.insert(soundName, sound);
}

void RpgSound::removeSound(const QString &soundName){
	QSoundEffect *sound = this->soundMap.take(soundName);
	if(sound != nullptr){
		if(sound->isPlaying()){
			sound->stop();
		}
		sound->deleteLater();
	}
}

void RpgSound::clearSound(){
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

void RpgSound::play(const QString &soundName, qreal volume, int times){
	QSoundEffect *sound = this->soundMap.value(soundName, nullptr);
	if(sound == nullptr){
		qDebug() << CodePath() << "Sound name is invalid or not exist.";
		return;
	}else{
		if(sound->status() == QSoundEffect::Error){
			qDebug() << CodePath() << "Sound status error.";
			return;
		}
		if(sound->isPlaying()){
			sound->stop();
		}
		sound->setLoopCount(times);
		sound->setVolume(volume);
		sound->play();
		emit this->started(soundName);
		qDebug() << CodePath() << "Sound effect: '" << sound->source().url(QUrl::PreferLocalFile) << "'";
	}
}

void RpgSound::stop(const QString &soundName){
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
