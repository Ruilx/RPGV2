#include "RpgSound.h"
#include <QtConcurrent>

RpgSound *RpgSound::_instance = nullptr;

void RpgSound::addSound(const QString &soundName, const QString &fileName){
	if(!QFile::exists(fileName)){
		qDebug() << CodePath() << "Sound file:'" << fileName << "' is not exist.";
	}
	QUrl source = QUrl::fromLocalFile(fileName);
	this->soundMap.insert(soundName, source);
}

void RpgSound::removeSound(const QString &soundName){
	this->soundMap.take(soundName);
}

void RpgSound::clearSound(){
	this->soundMap.clear();
}

void RpgSound::play(const QString &soundName, qreal volume, int times){
//	QSoundEffect *sound = this->soundMap.value(soundName, nullptr);
//	if(sound == nullptr){
//		qDebug() << CodePath() << "Sound name is invalid or not exist.";
//		return;
//	}else{
//		if(sound->status() == QSoundEffect::Error){
//			qDebug() << CodePath() << "Sound status error.";
//			return;
//		}
////		if(sound->isPlaying()){
////			sound->stop();
////		}
//		sound->setLoopCount(times);
//		sound->setVolume(volume);
//		sound->play();
//		emit this->started(soundName);
//		qDebug() << CodePath() << "Sound effect: '" << sound->source().url(QUrl::PreferLocalFile) << "'";
//	}
	QUrl url = this->soundMap.value(soundName, QUrl());
	qDebug() << url << this->soundMap;
	if(!url.isValid()){
		qDebug() << CodePath() << "Sound name is invalid or not exist.";
		return;
	}else{
		if(this->threadPool == nullptr){
			qDebug() << CodePath() << "Sound thread pool cannot find.";
			return;
		}
		QtConcurrent::run(this->threadPool, [this, volume, times, &soundName, url](){
//			if(QThread::currentThread() != this->thread()){
//				QMetaObject::invokeMethod(this, "play", Qt::QueuedConnection, Q_ARG(QString, soundName), Q_ARG(qreal, volume), Q_ARG(int, times));
//				return;
//			}
			if(this->threadPool->activeThreadCount() >= this->threadPool->maxThreadCount()){
				qDebug() << CodePath() << QString("Threadpool is full. %1/%2").arg(this->threadPool->activeThreadCount()).arg(this->threadPool->maxThreadCount());
				return;
			}
			QSoundEffect *sound = new QSoundEffect(nullptr);
			sound->setLoopCount(times);
			sound->setVolume(volume);
			sound->setSource(url);
			if(sound->status() == QSoundEffect::Error){
				qDebug() << CodePath() << "Sound status error.";
				sound->deleteLater();
				return;
			}
			sound->play();
			emit this->started(soundName);
			qDebug() << CodePath() << "Sound effect: '" << sound->source().url(QUrl::PreferLocalFile) << "'";
			QEventLoop loop;
			this->connect(sound, &QSoundEffect::playingChanged, [this, &loop, &sound](){
				if(sound->isPlaying() == false){
					loop.quit();
				}
			});
			loop.exec();
			qDebug() << CodePath() << "sound:" << sound << "loop finished.";
			sound->deleteLater();
			return;
		});
	}

}

void RpgSound::stop(const QString &soundName){
//	QSoundEffect *sound = this->soundMap.value(soundName, nullptr);
//	if(sound == nullptr){
//		qDebug() << CodePath() << "Sound name is invalid or not exist.";
//		return;
//	}else{
//		if(sound->isPlaying()){
//			sound->stop();
//			emit this->stopped(soundName);
//		}
//	}
	Q_UNUSED(soundName);
	qDebug() << CodePath() << "Sound changed cannot be stop manmully.";
}
