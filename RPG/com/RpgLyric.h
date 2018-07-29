#ifndef RPGLYRIC_H
#define RPGLYRIC_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/Global.h>
#include <RPG/com/RpgMusic.h>
#include <RPG/core/RpgObject.h>
#include <RPG/core/RpgFileManager.h>

class LyricEffect;
/**
 * @brief The RpgLyric class
 * RPGLyric类
 * 是歌词输出类, 在屏幕上运行歌词
 * 使用RpgMusic进行音乐播放, 并监控播放位置并过渡输出歌词
 * 继承RpgObject
 *
 *
 * 歌词格式 UTF-8格式:
 * [毫秒整数时间]一行歌词
 * [秒小数时间]一行歌词
 * [小时:分钟:秒.毫秒]一行歌词
 * [分钟:秒.毫秒]一行歌词
 * 其中 小时/分钟/秒必须是两位数, 毫秒必须是三位数
 *
 */
class RpgLyric : public RpgObject
{
	Q_OBJECT
private:
	QGraphicsTextItem *lyric = new QGraphicsTextItem(this);		// 显示歌词
	QGraphicsDropShadowEffect *lyricShadowEffect = new QGraphicsDropShadowEffect(this); // 歌词下的阴影
	QPropertyAnimation *lyricOpacityAnimation = new QPropertyAnimation(this->lyric, "opacity", this);

	const QString css = "r{ color: #FF3333; }g{ color: #33FF33; }b{ color: #9999FF; }y{ color: #EEEE33 }";

	QMap<int, QString> lyricMap;	// 顺序保存歌词的Map列表
	QMap<int, QString>::ConstIterator lyricI; // 顺序保存歌词的迭代器

	RpgMusic *musicObj = nullptr; // 音乐控件(拿取播放位置用)

	QPoint pos = QPoint(50, ScreenHeight - 65); // 默认歌词位置

	/**
	 * @brief lyricAnimationShow
	 * 显示歌词的歌词过渡动画
	 */
	void lyricAnimationShow();

	/**
	 * @brief lyricAnimationHide
	 * 隐藏歌词的歌词过渡动画
	 */
	void lyricAnimationHide();

public:
	/**
	 * @brief RpgLyric
	 * @param parentScene
	 * @param parent
	 * RPG Lyric控件, 设置父场景之后, 调用显示/退出显示的动画
	 */
	explicit RpgLyric(QGraphicsScene *parentScene = nullptr, QObject *parent = nullptr);

	/**
	 * @brief setRpgMusic
	 * @param music
	 * 设置需要反应的Music控件
	 */
	void setRpgMusic(RpgMusic *music);

	/**
	 * @brief loadLyric
	 * @param filename
	 * 读取歌词文件
	 */
	void loadLyricFromFile(const QString &filename);

	/**
	 * @brief loadLyric
	 * @param name
	 * 从RpgFileManager读取歌词文件
	 */
	void loadLyric(const QString &name);

	/**
	 * @brief exec
	 * 开始根据音乐播放时长显示相应歌词
	 */
	void exec();

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置默认文字颜色(CSS颜色除外)
	 */
	void setTextColor(const QColor &color){ this->lyric->setDefaultTextColor(color); }
	/**
	 * @brief setFont
	 * @param font
	 * 设置文本
	 */
	void setFont(const QFont &font){ this->lyric->setFont(font); }
	/**
	 * @brief setPos
	 * @param pos
	 * 设置歌词相对于窗口的pos
	 */
	void setPos(const QPoint &pos){ this->pos = pos; }

	/**
	 * @brief _dumpCurrentLyric
	 * 在debug窗口显示全部歌词
	 */
	inline void _dumpCurrentLyric(){
		qDebug() << CodePath() << "==================== Lyric ====================";
		for(QMap<int, QString>::ConstIterator i = this->lyricMap.constBegin(); i != this->lyricMap.constEnd(); i++){
			qDebug() << CodePath() << QString("[%1]%2").arg(i.key()).arg(i.value());
		}
		qDebug() << CodePath() << "===============================================";
	}

protected:
	/**
	 * @brief showLyric
	 * 开始执行歌词(由exec调用)
	 */
	void showLyric();

	/**
	 * @brief hideLyric
	 * 结束, 隐藏lyric控件(由Music的停止调用)
	 */
	void hideLyric();
};

#endif // RPGLYRIC_H
