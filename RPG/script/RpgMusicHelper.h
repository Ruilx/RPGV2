#ifndef RPGMUSICHELPER_H
#define RPGMUSICHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgMusic.h>

class RpgMusicHelper : public QObject
{
	Q_OBJECT

	RpgMusic *music = nullptr;
public:
	explicit RpgMusicHelper(RpgMusic *rpgMusic, QObject *parent = nullptr) : QObject(parent){
		if(rpgMusic == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr";
			return;
		}
		this->music = rpgMusic;
	}

	Q_INVOKABLE void addMusic(const QString &musicName, const QString &fileName){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->addMusic(musicName, fileName);
	}

	Q_INVOKABLE void removeMusic(const QString &musicName){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->removeMusic(musicName);
	}

	Q_INVOKABLE void clearMusic(){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->clearMusic();
	}

	Q_INVOKABLE bool isRunning(){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return false;
		}
		return this->music->isRunning();
	}

	Q_INVOKABLE QString getMusic(const QString &musicName){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return QString();
		}
		return this->music->getMusic(musicName);
	}

	Q_INVOKABLE void setVolume(int volume){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->setVolume(volume);
	}

	Q_INVOKABLE void setLoop(int loop){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->setLoop(loop);
	}

	Q_INVOKABLE void playMusic(const QString &musicName){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->playMusic(musicName);
	}

	Q_INVOKABLE void stopMusic(){
		if(this->music == nullptr){
			qDebug() << CodePath() << "RpgMusic is nullptr.";
			return;
		}
		this->music->stopMusic();
	}

signals:

public slots:
};

#endif // RPGMUSICHELPER_H
