#include "RpgMusic.h"

RpgMusic *RpgMusic::_instance = nullptr;

void RpgMusic::volumeTransition(bool upward, int duration){
	if(upward){
		this->volumeAnimation->setStartValue(0);
		this->volumeAnimation->setEndValue(this->volume);
	}else{
		this->volumeAnimation->setStartValue(this->volume);
		this->volumeAnimation->setEndValue(0);
	}
	this->volumeAnimation->setDuration(duration);
	this->volumeAnimation->start();
	QEventLoop loop;
	this->connect(this->volumeAnimation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
	loop.exec();
}

RpgMusic::RpgMusic(QObject *parent) : QObject(parent){
	this->music->setVolume(80);

	connect(this->music, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state){
		if(state == QMediaPlayer::PlayingState){
			if(this->currentLoop >= 0)
				qInfo() << CodePath() << "Start playing: '" << this->music->media().canonicalUrl().url(QUrl::PreferLocalFile) << "'";
			emit this->started();
			return;
		}else if(state == QMediaPlayer::StoppedState){
			if(this->loop > 0 && this->currentLoop >= this->loop){
				qInfo() << CodePath() << "Stopped: '" << this->music->media().canonicalUrl().url(QUrl::PreferLocalFile) << "'";
				emit this->stopped();
				this->currentLoop = 0;
				return;
			}
		}
	});

	connect(this->music, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status){
		if(status == QMediaPlayer::EndOfMedia){
			// 如果loop是无限循环, currentLoop是当前已经循环了几遍了, 如果loop有值, currentLoop要与其比较判断是否要继续播放下去
			if(this->loop < 0 || (this->loop > 0 && this->currentLoop < this->loop)){
				this->music->setPosition(0);
				this->music->play();
				currentLoop++;
			}
		}else if(status == QMediaPlayer::InvalidMedia){
			// 不可用的media, 报错
			qDebug() << CodePath() << "Media not vaild, cannot play the course.";
			return;
		}
	});

	this->music->setNotifyInterval(50);
	connect(this->music, &QMediaPlayer::positionChanged, [this](int milliseconds){
		//qDebug() << CodePath() << "Current Interval Position:" << milliseconds;
		emit this->seeked(milliseconds);
	});
}

void RpgMusic::addMusic(const QString &musicName, const QString &fileName){
	if(!QFile::exists(fileName)){
		qDebug() << CodePath() << "Music file:'" << fileName << "' is not exist.";
		return;
	}
	this->musicMap.insert(musicName, QUrl::fromLocalFile(fileName));
}

void RpgMusic::removeMusic(const QString &musicName){
	this->musicMap.remove(musicName);
}

void RpgMusic::clearMusic(){
	this->musicMap.clear();
}

bool RpgMusic::isRunning() const{
	return this->music->state() == QMediaPlayer::PlayingState;
}

QString RpgMusic::getMusic(const QString &musicName){
	if(!this->musicMap.contains(musicName)){
		qDebug() << CodePath() << "Music name:'" << musicName << "' is not registered.";
		return QString();
	}
	return this->musicMap.value(musicName).url(QUrl::PreferLocalFile);
}

void RpgMusic::setVolume(int volume){
	this->volume = volume;
	this->music->setVolume(volume);
}

void RpgMusic::playMusic(const QString &musicName){
	if(!this->musicMap.contains(musicName)){
		qDebug() << CodePath() << "Music name:'" << musicName << "' is not registered.";
		return;
	}
	if(this->music->state() != QMediaPlayer::PlayingState){
		this->stopMusic();
	}
	this->music->setMedia(QMediaContent(this->musicMap.value(musicName)));
	this->music->play();
	this->volumeTransition(true);
	this->currentLoop++;
}

void RpgMusic::stopMusic(){
	this->volumeTransition(false);
	this->music->stop();
	this->currentLoop = 0;
}
