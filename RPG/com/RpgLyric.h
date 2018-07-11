#ifndef RPGLYRIC_H
#define RPGLYRIC_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/Global.h>
#include <RPG/com/RpgMusic.h>
#include <RPG/core/RpgObject.h>

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

	QPoint pos = QPoint(50, ScreenHeight - 65);

	void lyricAnimationShow();

	void lyricAnimationHide();

public:
	/**
	 * @brief RpgLyric
	 * @param parentScene
	 * @param parent
	 * RPG Lyric控件, 设置父场景之后, 调用显示/退出显示的动画
	 */
	explicit RpgLyric(QGraphicsScene *parentScene = nullptr, QObject *parent = nullptr): RpgObject(parentScene, parent, nullptr){
		this->lyric->setDefaultTextColor(QColor(Qt::white));
		this->lyric->document()->setDefaultStyleSheet(this->css);

//		QFont font = Global::applicationFont;{
//			font.setFamily("aquafont");
//			font.setPixelSize(22);
//			font.setBold(false);
//		}

		QFont font = Global::applicationFont;{
			font.setFamily("Dukungan");
			font.setPixelSize(18);
			font.setBold(false);
		}

		this->lyric->setFont(font);

		this->setZValue(LyricZValue);
		this->lyric->setZValue(0.0f);

		this->lyricShadowEffect->setColor(QColor("#202020"));
		this->lyricShadowEffect->setBlurRadius(5.0f);
		this->lyricShadowEffect->setOffset(1.0f, 1.0f);
		this->lyric->setGraphicsEffect(this->lyricShadowEffect);

		this->lyricOpacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
		this->lyricOpacityAnimation->setStartValue(0.0f);
		this->lyricOpacityAnimation->setEndValue(1.0f);
		this->lyricOpacityAnimation->setDuration(300);
		this->lyricOpacityAnimation->setLoopCount(1);

	}

	void setRpgMusic(RpgMusic *music);

	void loadLyric(const QString &filename);

	void exec();

	void setTextColor(const QColor &color){ this->lyric->setDefaultTextColor(color); }
	void setFont(const QFont &font){ this->lyric->setFont(font); }
	void setPos(const QPoint &pos){ this->pos = pos; }

	inline void _dumpCurrentLyric(){
		qDebug() << CodePath() << "==================== Lyric ====================";
		for(QMap<int, QString>::ConstIterator i = this->lyricMap.constBegin(); i != this->lyricMap.constEnd(); i++){
			qDebug() << CodePath() << QString("[%1]%2").arg(i.key()).arg(i.value());
		}
		qDebug() << CodePath() << "===============================================";
	}

protected:
	void showLyric();

	void hideLyric();
};

#endif // RPGLYRIC_H
