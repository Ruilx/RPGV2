#ifndef RPGMUSIC_H
#define RPGMUSIC_H

#include <QtCore>
#include <QtMultimedia>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgFileManager.h>

/**
 * @brief The RpgMusic class
 * RPGMusic类, 是进行RPG游戏的时候输出游戏声音的组件(BGM)
 * 是一个持续的声音, 所以只能被复用一次, 所有场景都可以使用这一个声音, 所以作为/单例模式/出现
 * 进行另一个声音的播放时, 前一个声音将会停止, 默认声音会一直循环播放
 *
 * 首先需要载入声音文件, 并给其取一个名字, 播放的时候只需要提供名字即可.
 *
 * 2018/06/05 02:03
 * (Windows)
 * 注: RpgMusic控件是由QMultiMedia组件进行实现, 组件将数据移交至系统的DirectShow进行处理
 * 则这时如果系统的DirectShow没有既定的Decoder, 则会报告doRender失败, 没有解决的错误代码
 * 0x80040266(MS解释:VFW_E_NO_TRANSPORT)
 * # Pins cannot connect because they don't support the same transport.
 * # For example, the upstream filter might require the IAsyncReader interface,
 * # while the downstream filter requires IMemInputPin.
 * # https://msdn.microsoft.com/en-us/library/windows/desktop/dd375623(v=vs.85).aspx
 * 所以需要安装一个可供DirectShow播放的DirectShow解码器
 * 最后选择了LAV Filters, 下载后使用regsvr32对AX文件进行注册, 然后DirectShow就可以进行解码了
 * (可支持的项目: 320K的MP3, OGG容器, MIDI等)
 *
 * 2018/07/27 14:48
 * Add和remove歌曲函数被删除, 因为Music接下来的文件管理交给RpgFileManager进行管理
 */
class RpgMusic : public QObject
{
	Q_OBJECT
	static RpgMusic *_instance;

	// 使用RpgFileManager管理音乐文件文件名, 取消自管
//	QHash<QString, QUrl> musicMap;
	QMediaPlayer *music = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
	int volume = 100;
	QPropertyAnimation *volumeAnimation = new QPropertyAnimation(this->music, "volume", this);

	int loop = -1;
	int currentLoop = 0;

	/**
	 * @brief volumeTransition 歌曲开始或停止的音量过渡
	 * @param upward 是否向音量增大方向过渡(T: 音量增大, F: 音量减小)
	 * @param duration 过渡持续时间
	 */
	void volumeTransition(bool upward, int duration = 250);
public:
	/**
	 * @brief instance 获取单例对象
	 * @return Self
	 */
	static RpgMusic *instance(){
		if(_instance == nullptr){
			_instance = new RpgMusic(nullptr);
		}
		return _instance;
	}

	/**
	 * @brief RpgMusic 构造函数
	 * @param parent 父QObject
	 */
	explicit RpgMusic(QObject *parent = nullptr);

	~RpgMusic(){
		if(this->isRunning()){
			this->stopMusic();
		}
	}

	/**
	 * @brief addMusic 增加一首歌曲
	 * @param musicName 指定一个歌曲名
	 * @param fileName 指定文件名
	 * 已移交给RpgFileManager进行管理
	 */
//	void addMusic(const QString &musicName, const QString &fileName);

	/**
	 * @brief removeMusic 删除一首歌曲
	 * @param musicName 指定歌曲名
	 * 已移交给RpgFileManager进行管理
	 */
//	void removeMusic(const QString &musicName);

	/**
	 * @brief clearMusic 删除歌单
	 * 已移交给RpgFileManager进行管理
	 */
//	void clearMusic();

	/**
	 * @brief isRunning 是否在工作
	 * @return :bool 工作中
	 */
	bool isRunning() const;

	/**
	 * @brief getMusic 获得歌曲名对应的文件
	 * @param musicName 指定歌曲名
	 * @return :QString 歌曲地址(QUrl::PreferLocalFile)
	 * 此函数被改为从RpgFileManager取出这个歌曲的fileName
	 */
	QString getMusic(const QString &musicName);

	/**
	 * @brief setVolume 设定音量
	 * @param volume 音量(0-100)
	 */
	void setVolume(int volume);

	/**
	 * @brief getVolume 获得音量(0-100)
	 */
	int getVolume() const { return this->volume; }

	/**
	 * @brief setLoop 设定循环次数
	 * @param loop 次数(1-x, -1(持续循环))
	 */
	inline void setLoop(int loop = -1){ this->loop = loop == 0 ? 1 : loop; }

	/**
	 * @brief getLoop 获得当前循环次数
	 */
	inline int getLoop() const { return this->loop; }

	/**
	 * @brief setNotifyInterval
	 * @param ms
	 * 设定播放心跳信号(默认1000ms)(给歌词的话一般50ms)
	 */
	void setNotifyInterval(int ms){
		this->music->setNotifyInterval(ms);
	}

signals:
	void started();
	void stopped();
	void seeked(int position);
public slots:
	/**
	 * @brief playMusic 运行歌曲
	 * @param musicName 指定一个歌曲名
	 * 从RpgFileManager取出文件信息
	 */
	void playMusic(const QString &musicName);

	/**
	 * @brief stopMusic 停止当前歌曲
	 */
	void stopMusic();
};

#endif // RPGMUSIC_H
