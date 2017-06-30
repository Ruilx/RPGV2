#ifndef RPGBANNER_H
#define RPGBANNER_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>

/**
 * @brief The RpgBanner class
 * RPGBanner类
 * 是在GraphicsScene背景图层中显示指定背景内容
 * 是一个单独线程控制显示
 * 设定好背景图片之后, 就可以看到背景图片按照指定时间淡入或淡出
 */

class RpgBanner : public QObject
{
	Q_OBJECT
	
	bool isRunning = false;
	//指定显示在哪个Scene上
	QGraphicsScene *parentScene = nullptr;
	//构成
	QGraphicsPixmapItem *background = new QGraphicsPixmapItem(nullptr);
	QGraphicsPixmapItem *foreground = new QGraphicsPixmapItem(this->background);
	
	QGraphicsOpacityEffect *foregroundEffect = new QGraphicsOpacityEffect(this);
	QPropertyAnimation *foregroundAnimation = new QPropertyAnimation(this->foregroundEffect, "opacity", this);
	
	QPointF viewportOffset = QPoint(0, 0);

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
	int speed = SpeedNormal;

	explicit RpgBanner(QGraphicsScene *parentScene, QObject *parent = nullptr): QObject(parent){
		this->parentScene = parentScene;
		this->setBackgroundColor(QColor(Qt::black));
		this->background->setZValue(BackgroundZValue);
		this->foreground->setZValue(0.1f);
		this->foreground->setGraphicsEffect(this->foregroundEffect);
		this->setStartOpacity(0.0f);
		this->setEndOpacity(1.0f);
		this->connect(this->foregroundAnimation, &QPropertyAnimation::finished, this, &RpgBanner::animationFinished);
	}

	RpgBanner(qreal startOpacity, qreal endOpacity, QGraphicsScene *parentScene, QObject *parent = nullptr){
		RpgBanner(parentScene, parent);
		this->setStartOpacity(startOpacity);
		this->setEndOpacity(endOpacity);
		this->setEasingCurveType(QEasingCurve::OutQuad);
	}

	/**
	 * @brief setForegroundPixmap
	 * @param pixmap
	 * 设置前端图片Pixmap, 如果不是屏幕大小的会放大到屏幕大小, 并且只裁剪中间一段
	 */
	void setForegroundPixmap(const QPixmap &pixmap){
		QPixmap _t;
		if(this->parentScene == nullptr){
			qDebug() << CodePath() << "ParentScene is nullptr before calling setForegroundPixmap.";
			return;
		}
		_t = pixmap.scaled(this->parentScene->sceneRect().size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
		QSize sceneSize = QSize(sceneSize.width(), sceneSize.height());
		if(_t.width() == this->parentScene->width()){
			_t = _t.copy(0, (_t.height() - sceneSize.height()) >> 1, _t.width(), sceneSize.height());
		}else if(_t.height() == this->parentScene->height()){
			_t = _t.copy((_t.width() - sceneSize.width()) >> 1, 0, sceneSize.width(), _t.height());
		}else{
			_t = _t.copy((_t.width() - sceneSize.width()) >> 1,
						 (_t.height() - sceneSize.height()) >> 1,
						 sceneSize.width(),
						 sceneSize.height());
		}
		this->foreground->setPixmap(_t);
	}

	/**
	 * @brief setBackgroundColor
	 * @param color
	 * 设置背景颜色, 从黑过渡或者从白过渡
	 */
	void setBackgroundColor(const QColor &color){
		if(this->parentScene == nullptr){
			qDebug() << CodePath() << "ParentScene is nullptr before calling setBackgroundColor.";
			return;
		}
		QPixmap _t(this->parentScene->width(), this->parentScene->height());
		_t.fill(color);
		this->background->setPixmap(_t);
	}
	
	/**
	 * @brief setSpeed
	 * @param milliseconds
	 * 设置淡入或淡出速度(毫秒)
	 */
	void setSpeed(int milliseconds){
		//this->foregroundAnimation->setDuration(milliseconds);
		this->speed = milliseconds;
	}
	
	/**
	 * @brief setParentScene
	 * @param parentScene
	 * 设置父Scene
	 */
	void setParentScene(QGraphicsScene *parentScene){
		this->parentScene = parentScene;
	}

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
	void setEasingCurveType(QEasingCurve::Type type){
		this->foregroundAnimation->setEasingCurve(QEasingCurve(type));
	}

	/**
	 * @brief setViewportOffset
	 * @param offset
	 * 设置当前SceneBoundingLeftTopPoint
	 */
	void setViewportOffset(const QPointF &offset){
		this->viewportOffset = offset;
	}

	/**
	 * @brief exec
	 * 开始执行
	 */
	void exec(){
		if(this->parentScene == nullptr){
			qDebug() << "RpgBanner::exec(): parentScene is not set. (Null)";
			return;
		}
		if(this->background->pixmap().isNull()){
			qDebug() << "RpgBanner::exec(): backgroundPixmap is null.";
			return;
		}
		if(this->foreground->pixmap().isNull()){
			qDebug() << "RpgBanner::exec(): foregroundPixmap is null.";
			return;
		}
		if(this->isRunning == true){
			qDebug() << "RpgBanner::exec(): RpgBanner is Running, please don't call it repeatly!";
			return;
		}
		this->background->setPos(this->parentScene->sceneRect().topLeft());
		this->foreground->setPos(this->parentScene->sceneRect().topLeft());
		this->foregroundAnimation->setDuration(this->speed);

		this->showBanner();
	}

	/**
	 * @brief showBanner
	 * 显示Banner
	 */
	void showBanner(){
		this->isRunning = true;
		emit this->enterAutoMode();
		this->parentScene->addItem(this->background);
		this->foregroundAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}

	/**
	 * @brief hideBanner
	 * 隐藏和删除Banner(删除背景的时候用)
	 */
	void hideBanner(){
		this->background->hide();
		this->parentScene->removeItem(this->background);
	}

	/**
	 * @brief finished
	 * 动画结束的时候, 停止动画, 直接跳至动画结尾, 并且退出自动模式
	 */
	void finished(){
		if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
			this->foregroundAnimation->setCurrentTime(this->foregroundAnimation->totalDuration());
			this->foregroundAnimation->stop();
		}
		emit this->quitAutoMode();
		this->isRunning = false;
	}

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
	void receiveKey(int key, Qt::KeyboardModifiers mod){
		qDebug() << tr("RpgBanner::receiveKey(): receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());
		//如果按键中断, 直接过渡结束
		if(key == Qt::Key_Return){
			this->finished();
		}
	}
};

#endif // RPGBANNER_H
