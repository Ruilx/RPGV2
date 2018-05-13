#ifndef RPGOBJECT_H
#define RPGOBJECT_H

#include <QtCore>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <RPG/Global.h>

/**
 * @brief The RpgObject class
 * RPGObject类
 * 是在QGraphicsScene中的组件的基类, 用来基本控制组件的运行
 */
class RpgObject : public QGraphicsObject
{
	Q_OBJECT

	bool _isRunning = false;

	QGraphicsScene *parentScene = nullptr;
protected:
	/**
	 * @brief boundingRect
	 * @return
	 * 抽象类没有实际的大小, 只用固定其在屏幕上的相对位置
	 */
	QRectF boundingRect() const{
		return QRectF();
	}

	/**
	 * @brief paint
	 * @param painter
	 * @param option
	 * @param widget
	 * 不用的继承类, 没有需要绘图的必要
	 */
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
		Q_UNUSED(painter)
		Q_UNUSED(option)
		Q_UNUSED(widget)
	}

	/**
	 * @brief getParentScene
	 * @return
	 * 得到所设置的Scene
	 */
	QGraphicsScene *getParentScene() const{
		return this->parentScene;
	}

public:
	/**
	 * @brief RpgObject
	 * @param parentScene
	 * @param parent
	 * @param parentItem
	 * 构造函数, 设置此Object的位置和父Item
	 */
	RpgObject(QGraphicsScene *parentScene, QObject *parent, QGraphicsObject *parentItem): QGraphicsObject(parentItem){
		if(parentScene == nullptr){
			qCritical() << CodePath() << "parentScene is nullptr.";
			return;
		}
		this->parentScene = parentScene;
		this->setParent(parent);

		this->setPos(this->parentScene->sceneRect().topLeft());
		this->hide();
		this->parentScene->addItem(this);
	}

	/**
	 * @brief setProcessing
	 * @param isRunning
	 * 设置此Object是否在工作, 并获取Event和输入
	 */
	void setProcessing(bool isRunning){
		this->_isRunning = isRunning;
	}

	/**
	 * @brief getProcessing
	 * @return
	 * 获取此Object是否在工作
	 */
	bool getProcessing() const{
		return this->_isRunning;
	}

	/**
	 * @brief setParentScene
	 * @param parentScene
	 * 获得此Object的父Scene
	 */
	inline void setParentScene(QGraphicsScene *parentScene){
		this->parentScene = parentScene;
	}

	/**
	 * @brief exec
	 * 执行[虚函数]
	 */
	virtual void exec() = 0;

	/**
	 * @brief receiveKey
	 * @param key
	 * @param mod
	 * 获得用户输入[虚函数]
	 */
	virtual void receiveKey(int key, Qt::KeyboardModifiers mod){
		if(!this->_isRunning){
			return;
		}
		qDebug() << CodePath() << tr("Receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());

	}

};

#endif // RPGOBJECT_H
