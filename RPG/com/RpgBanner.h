#ifndef RPGBANNER_H
#define RPGBANNER_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/core/RpgObject.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgFileManager.h>

/**
 * @brief The RpgBanner class
 * RPGBanner类
 * 是在GraphicsScene背景图层中显示指定背景内容
 * 是一个单独线程控制显示
 * 设定好背景图片之后, 就可以看到背景图片按照指定时间淡入或淡出
 * -> dlgasitem 新建
 * 2018/05/10 RpgBanner 继承QGraphicsItem, 让自己以Item的方式出现在界面中
 * QGraphicsObject = QGraphicsItem + QObject
 * 另, 将自己变为QGraphicsObject之后, this的pos设置成了view的左上角, 也就是说box的pos只需要设置成relative position即可
 * 2018/05/13 RpgBanner 继承RpgObject, 公共部分的Item由RpgObject承担
 */

class RpgBanner : public RpgObject
{
	Q_OBJECT
public:
	/**
	 * @brief The Speed enum
	 * 过渡速度, 预先定义淡入和淡出速度
	 */
	enum Speed{
		SpeedSlow = 500,
		SpeedNormal = 250,
		SpeedFast = 150,
		SpeedInfinitly = 0
	};

	/**
	 * @brief The Layer enum
	 * 显示Banner在哪一层, 全局最下还是全局最上
	 */
	enum Layer{
		BottomLayer = 0,
		TopLayer = 1,
	};
private:
	//指定显示在哪个Scene上
	//QGraphicsScene *parentScene = nullptr;
	//构成
	QGraphicsPixmapItem *background = new QGraphicsPixmapItem(this);
	QGraphicsPixmapItem *foreground = new QGraphicsPixmapItem(this->background);
	
	QGraphicsOpacityEffect *foregroundEffect = new QGraphicsOpacityEffect(this);
	QPropertyAnimation *foregroundAnimation = new QPropertyAnimation(this->foregroundEffect, "opacity", this);
	
//	QPointF viewportOffset = QPoint(0, 0);

	int speed = SpeedNormal;
	Layer layer = BottomLayer;

	bool canBeInterrupted = false;

public:
	/**
	 * @brief RpgBanner
	 * @param parentScene
	 * @param parent
	 * RPG Banner控件, 设置父场景之后, 调用显示/退出显示的动画
	 */
	explicit RpgBanner(QGraphicsScene *parentScene, QObject *parent = nullptr);

	/**
	 * @brief RpgBanner
	 * @param startOpacity
	 * @param endOpacity
	 * @param parentScene
	 * @param parent
	 * 暂时不用, 由Scene来控制
	 */
	RpgBanner(qreal startOpacity, qreal endOpacity, QGraphicsScene *parentScene, QObject *parent = nullptr);

	/**
	 * @brief setForegroundPixmap
	 * @param pixmap
	 * 设置前端图片Pixmap, 如果不是屏幕大小的会放大到屏幕大小, 并且只裁剪中间一段
	 */
	void setForegroundPixmap(const QPixmap &pixmap);

	/**
	 * @brief setForegroundPixmap
	 * @param name
	 * 设置前端图片, 使用name在fileManager中找到对应URL然后载入, 如果不是屏幕大小的会放大到屏幕大小, 并且只裁剪中间一段
	 */
	void setForegroundPixmap(const QString &name){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::ImageFile, name);
		if(!filename.isEmpty()){
			this->setForegroundPixmap(QPixmap(filename));
		}
	}

	/**
	 * @brief setBackgroundColor
	 * @param color
	 * 设置背景颜色, 从黑过渡或者从白过渡(其实可以随意选择颜色)
	 */
	void setBackgroundColor(const QColor &color);

	/**
	 * @brief setBackgroundColor
	 * @param color
	 * 设置背景颜色, 从黑过渡或者从白过渡(其实可以随意选择颜色), 接受字符串颜色, #开头的16进制数或者颜色名称"red","blue","green","yellow"等
	 */
	void setBackgroundColor(const QString &color){
		this->setBackgroundColor(QColor(color));
	}
	
	/**
	 * @brief setBackgroundPixmap
	 * @param pixmap
	 * 设置背景图片(扩展)
	 */
	void setBackgroundPixmap(const QPixmap &pixmap);

	/**
	 * @brief setBackgroundPixmap
	 * @param name
	 * 设置背景图片(扩展), 使用name在fileManager中找到对应URL然后载入
	 */
	void setBackgroundPixmap(const QString &name){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::ImageFile, name);
		if(!filename.isEmpty()){
			this->setBackgroundPixmap(QPixmap(filename));
		}
	}

	/**
	 * @brief setBackgroundWithForeground
	 * 将此时的前端图片显示在背景上, 前端图片会变为透明, setStartOpacity置为0, setEndOpacity置为1
	 */
	void setBackgroundWithForeground(){
		QPixmap _t = this->foreground->pixmap();
		this->background->setPixmap(_t);
		_t.fill(Qt::transparent);
		this->foreground->setPixmap(_t);
		this->setStartOpacity(0.0f);
		this->setEndOpacity(1.0f);
	}
	/**
	 * @brief setSpeed
	 * @param milliseconds
	 * 设置淡入或淡出速度(毫秒)
	 */
	inline void setSpeed(int milliseconds){ this->speed = milliseconds; }
	
	/**
	 * @brief setParentScene
	 * @param parentScene
	 * 设置父Scene
	 */
	//inline void setParentScene(QGraphicsScene *parentScene){ this->setParentScene(parentScene); }

	/**
	 * @brief setStartOpacity
	 * @param opacity
	 * 设置开始透明值
	 */
	void setStartOpacity(qreal opacity){
		if(opacity > 1.0f || opacity < 0.0f){
			qDebug() << "RpgBanner::setStartOpacity: value:" << opacity << "out of range.";
			return;
		}
		this->foregroundAnimation->setStartValue(opacity);
		this->foregroundEffect->setOpacity(opacity);
	}

	/**
	 * @brief setEndOpacity
	 * @param opacity
	 * 设置结束透明值
	 */
	void setEndOpacity(qreal opacity){
		if(opacity > 1.0f || opacity < 0.0f){
			qDebug() << "RpgBanner::setEndOpacity: value:" << opacity << "out of range.";
			return;
		}
		this->foregroundAnimation->setEndValue(opacity);
	}

	/**
	 * @brief setEasingCurveType
	 * @param type
	 * 设置出现加速度值的type
	 * @sa QEasingCurve::Type
	 */
	void setEasingCurveType(QEasingCurve::Type type){ this->foregroundAnimation->setEasingCurve(QEasingCurve(type)); }

	/**
	 * @brief setCanBeInterrupt
	 * @param able
	 * 设置在过渡动画期间用户按下Enter键是否能打断过渡
	 * able == true 可以打断(默认不能)(而且执行结束之后会重置这个打断状态为无效, 也就是不能打断)(为了防止用户滥用Enter键)
	 */
	void setCanBeInterrupt(bool able){ this->canBeInterrupted = able; }

//	/**
//	 * @brief setViewportOffset
//	 * @param offset
//	 * 设置当前SceneBoundingLeftTopPoint
//	 */
//	void setViewportOffset(const QPointF &offset){
//		this->viewportOffset = offset;
//	}

	/**
	 * @brief exec
	 * 开始执行
	 */
	void exec();

	/**
	 * @brief execExit
	 * 执行相反的退出函数
	 */
	void execExit();

	int waitingForBannerComplete(){
		if(!this->getProcessing()){
			qDebug() << CodePath() << ": Not running yet, cannot wait.";
			return -1;
		}
		QEventLoop eventLoop(this);
		this->connect(this, &RpgBanner::quitAutoMode, &eventLoop, &QEventLoop::quit);
		if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
			eventLoop.exec();
		}
		return 0;
	}

protected:
	/**
	 * @brief showBanner
	 * 显示Banner
	 */
	void showBanner();

	/**
	 * @brief hideBanner
	 * 隐藏和删除Banner(删除背景的时候用)
	 */
	void hideBanner();

	/**
	 * @brief finished
	 * 动画结束的时候, 停止动画, 直接跳至动画结尾, 并且退出自动模式
	 */
	void finished();

signals:
	/**
	 * @brief enterAutoMode
	 * 信号: 进入自动执行模式
	 */
	void enterAutoMode();
	/**
	 * @brief quitAutoMode
	 * 信号: 退出自动执行模式
	 */
	void quitAutoMode();
private slots:
	void animationFinished(){
		this->finished();
	}

public slots:
	void receiveKey(int key, Qt::KeyboardModifiers mod);
};

#endif // RPGBANNER_H
