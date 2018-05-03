#ifndef RPGMUSIC_H
#define RPGMUSIC_H

#include <QtCore>
#include <QtMultimedia>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>

/**
 * @brief The RpgMusic class
 * RPGMusic类, 是进行RPG游戏的时候输出游戏声音的组件(BGM)
 * 是一个持续的声音, 所以只能被复用一次, 所有场景都可以使用这一个声音, 所以作为/单例模式/出现
 * 进行另一个声音的播放时, 前一个声音将会停止, 默认声音会一直循环播放
 *
 * 首先需要载入声音文件, 并给其取一个名字, 播放的时候只需要提供名字即可.
 */
class RpgMusic : public QObject
{
	Q_OBJECT
	static RpgMusic *_instance;

	QHash<QString, QUrl> musicMap;
	QMediaPlayer *music = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
	int volume = 100;
	QPropertyAnimation *volumeAnimation = new QPropertyAnimation(this->music, "volume", this);

	int loop = -1;
	int currentLoop = 0;

	void volumeTransition(bool upward, int duration = 250){
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
public:
	static RpgMusic *instance(){
		if(_instance == nullptr){
			_instance = new RpgMusic(nullptr);
		}
		return _instance;
	}

	explicit RpgMusic(QObject *parent = nullptr) : QObject(parent){
		this->music->setVolume(100);

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
			}
		});
	}

	~RpgMusic(){
		if(this->isRunning()){
			this->stopMusic();
		}
	}

	void addMusic(const QString &musicName, const QString &fileName){
		if(!QFile::exists(fileName)){
			qDebug() << CodePath() << "Music file:'" << fileName << "' is not exist.";
		}
		this->musicMap.insert(musicName, QUrl(fileName));
	}

	void removeMusic(const QString &musicName){
		this->musicMap.remove(musicName);
	}

	void clearMusic(){
		this->musicMap.clear();
	}

	bool isRunning() const{
		return this->music->state() == QMediaPlayer::PlayingState;
	}

	QString getMusic(const QString &musicName){
		if(!this->musicMap.contains(musicName)){
			qDebug() << CodePath() << "Music name:'" << musicName << "' is not registered.";
			return QString();
		}
		return this->musicMap.value(musicName).url(QUrl::PreferLocalFile);
	}

	void setVolume(int volume){
		this->volume = volume;
		this->music->setVolume(volume);
	}

	void setLoop(int loop = -1){
		this->loop = loop;
	}

signals:
	void started();
	void stopped();
public slots:
	void playMusic(const QString &musicName){
		if(!this->musicMap.contains(musicName)){
			qDebug() << CodePath() << "Music name:'" << musicName << "' is not registered.";
			return;
		}
		if(this->music->state() != QMediaPlayer::PlayingState){
			this->stopMusic();
		}
		this->music->setMedia(this->musicMap.value(musicName));
		this->music->play();
		this->volumeTransition(true);
	}

	void stopMusic(){
		this->volumeTransition(false);
		this->music->stop();
		this->currentLoop = 0;
	}
};

#endif // RPGMUSIC_H
