#ifndef RPGSOUND_H
#define RPGSOUND_H

#include <QtCore>
#include <QSoundEffect>
#include <RPG/Global.h>

/**
 * @brief The RpgSound class
 * RPGSound类, 是进行RPG游戏的时候输出音效的组件(SE)
 * 因为QSoundEffect(已经低延迟了)只支持WAV且不能预先加载(或者decode至内存), 所以运行的时候可能会慢一点
 * 之后可能以ThreadPool线程池进行play, 速度应该会快一点
 * 音效在play时可指定循环次数以及音效大小.
 * 不同音效可以同时运行, 相同音效同时(时间很接近)播放会打断上一次播放(因为Hash保存的是对象)
 * 之后可能已文件名来保存Hash
 *
 * 首先要载入声音(WAV)文件, 并给其取一个名字, 播放的时候只需要提供名字即可
 */

class RpgSound : public QObject
{
	Q_OBJECT
	static RpgSound *_instance;

	QHash<QString, QSoundEffect*> soundMap;
public:
	/**
	 * @brief instance 获取单例对象
	 * @return Self
	 */
	static RpgSound *instance(){
		if(_instance == nullptr){
			_instance = new RpgSound(nullptr);
		}
		return _instance;
	}

	/**
	 * @brief RpgSound 构造函数
	 * @param parent 父QObject
	 */
	explicit RpgSound(QObject *parent = nullptr) : QObject(parent){
		this->soundMap.clear();
	}

	~RpgSound(){
		for(QSoundEffect *e: this->soundMap){
			if(e != nullptr){
				if(e->isPlaying()){
					e->stop();
				}
				e->deleteLater();
			}
		}
	}

	/**
	 * @brief addSound 增加一个音效
	 * @param soundName 指定一个音效名
	 * @param fileName 指定文件名
	 */
	void addSound(const QString &soundName, const QString &fileName);

	/**
	 * @brief removeSound 删除一个音效
	 * @param soundName 指定音效名
	 */
	void removeSound(const QString &soundName);

	/**
	 * @brief clearSound 删除音效列表
	 */
	void clearSound();
signals:
	void started(const QString &soundName);
	void stopped(const QString &soundName);
public slots:
	/**
	 * @brief play 运行音效
	 * @param soundName 音效名
	 * @param volume 音量(0.0-1.0)
	 * @param times 音效循环次数
	 */
	void play(const QString &soundName, qreal volume = 0.7f, int times = 1);

	/**
	 * @brief stop 停止运行音效
	 * @param soundName 音效名
	 */
	void stop(const QString &soundName);
};

#endif // RPGSOUND_H
