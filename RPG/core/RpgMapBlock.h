#ifndef RPGMAPBLOCK_H
#define RPGMAPBLOCK_H

#include <QtCore>
#include <QGraphicsPixmapItem>
#include <RPG/core/RpgTileSetBase.h>
#include <RPG/com/RpgScene.h>
#include <RPG/Global.h>

/**
 * @brief The RpgMapBlock class
 * 地图上的块, 单独的形成一个背景, 一般是作为背景的物件, 窗口, 窗帘, 柜子, 桌子和其他的物品的东西
 * 如果物块大于1x1, 将会被分解成1x1的块, 由于游戏中每一行都需要设置不同的ZValue, 保证上一层的玩家精灵
 * 会被下一层的物件挡住.
 * RpgMapBlock是一个场景放置块类, 输入一个Location, 内容, 是否让精灵通过, parent(为什么要parent?)
 * 内容可以为空, 如果是那一种一段时间变化的的动图, 可以使用addPixmap添加帧
 * 现在正在想怎么让能动的背景的运动方式timer是主类统一发信号还是自我timer对象进行控制? 最后决定自我timer对象进行控制
 * (一个QTimeLine和一堆QTimeLine的问题)
 *
 * 关于ZValue的问题已经在Global文件中预先定义.
 *
 * 有一个注意的: 如果物品(假设柜子)占有3层高, 但最后一层没有占满地板, 则最后一层是可通过的, 而且ZValue和地面值范围相同
 * 最高一层也能通过, ZValue和物品值范围相同, 中间一层ZValue高于地板, 但不能通过
 *
 *   +===========+
 *   |           | <- 能通过, ZValue值比较高
 *   +-----------+
 *   |           | <- 不能通过, ZValue值比较高
 *   +-----------+
 *   |    ***    | <- 能通过, ZValue值比较低
 *   +====***====+
 *        ***      <- 主角, ZValue等于其所踩格子的行数 + 0.4
 */
class RpgMapBlock: public QGraphicsPixmapItem, public QObject
{
	/**
	 * @brief mapPixmapList
	 * 这个就是存储的图像序列
	 */
	QList<QPixmap *> mapPixmapList;
	/**
	 * @brief currentIndex
	 * 这个是现在正在显示的图片的Index
	 */
	int currentIndex = 0;
	/**
	 * @brief canWalkThrough
	 * 这里存储着精灵是否能进入此处(通过)
	 */
	bool canWalkThrough = true;
	/**
	 * @brief parent
	 * 父类Scene
	 */
	RpgScene *parentScene = nullptr;

	QTimeLine *timeLine = new QTimeLine(1000, this);
	/**
	 * @brief location
	 * 这个存的是块地址, 需要像素地址请乘以MapBlockWidth/Height或直接调用this->pos().
	 */
	QPoint location = QPoint(0, 0);

	/**
	 * @brief setPixmapPrivate
	 * @param content Pixmap 显示的图片内容
	 * @param append Bool 那个...是否要添加在那个图片列表的后面
	 * 这个函数是构造函数们对于接入的参数方式统一进行数据处理的地方
	 * 可能是一个静态图片, 可能是一个动图(由addPixmap添加, 则这里是一帧图像), 然后写入数据结构中
	 */
	void setPixmapPrivate(const QPixmap &content, bool append = false){
		if(!content.isNull()){
			if(content.width() != MapBlockWidth || content.height() != MapBlockHeight){
				qDebug() << QObject::tr("RpgMapBlock::setPixmapPrivate(): content is not fit at (%1, %2), source: (%1, %2)")
							.arg(MapBlockWidth).arg(MapBlockHeight).arg(content.width()).arg(content.height());
			}
			if(append == true){
				this->addPixmap(content);
			}else{
				this->clearPixmap();
				this->addPixmap(content);
			}
			this->showPixmapIndex(0);
		}else{
			qDebug() << "RpgMapBlock::setPixmapPrivate: Pixmap is Null." << CodePath();
		}
	}

protected:
	/**
	 * @brief setPixmap
	 * @param content
	 * 继承要用的? 或者构造函数调用用的
	 * 继承函数里有了, 删除掉
	 */
	//void setPixmap(const QPixmap &content){
		//this->setPixmapPrivate(content);
	//}

public:
	/**
	 * @brief RpgMapBlock
	 * @param location Point: 位置
	 * @param content Pixmap: 内容
	 * @param canWalkThrough Bool: 设置能否走进该区域
	 * @param parentScene: RpgScene: 父场景指针, 显示到哪个场景中
	 * @param parentGraphicsItem: QGraphicsItem: 父GraphicsItem, 继承要用的
	 * @param parent: QObject父类
	 */
	RpgMapBlock(const QPoint &location,
				const QPixmap &content = QPixmap(),
				bool canWalkThrough = true,
				RpgScene *parentScene = nullptr,
				QGraphicsItem *parentGraphicsItem = nullptr,
				QObject *parent = nullptr): QGraphicsPixmapItem(parentGraphicsItem), QObject(parent){
		this->parentScene = parentScene;
		this->location.setX(location.x());
		this->location.setY(location.y());
		this->setPos(location);
		this->canWalkThrough = canWalkThrough;
		this->setPixmapPrivate(content, true);
		connect(this->timeLine, &QTimeLine::frameChanged, this, [this](int frame){
			if(this->mapPixmapList.length() <= 1 && this->timeLine->state() != QTimeLine::NotRunning){
				this->timeLine->stop();
			}
			if(frame < this->mapPixmapList.length() && frame >= 0){
				showPixmapNext();
			}
		});
	}

	/**
	 * @brief RpgMapBlock
	 * @param col Int: 列(按照块的宽度倍数)
	 * @param row Int: 行(按照块的宽度倍数)
	 * @param content Pixmap: 内容
	 * @param canWalkThrough Bool: 设置能否走进该区域
	 * @param parentScene: RpgScene: 父场景指针, 显示到哪个场景中
	 * @param parentGraphicsItem: QGraphicsItem: 父GraphicsItem, 继承要用的
	 * @param parent: QObject父类
	 */
	RpgMapBlock(int col, int row,
				const QPixmap &content = QPixmap(),
				bool canWalkThrough = true,
				RpgScene *parentScene = nullptr,
				QGraphicsItem *parentGraphicsItem = nullptr,
				QObject *parent = nullptr): QGraphicsPixmapItem(parentGraphicsItem), QObject(parent){
		this->parentScene = parentScene;
		this->location.setX(col);
		this->location.setY(row);
		this->setPos(col * MapBlockWidth, row * MapBlockHeight);
		this->canWalkThrough = canWalkThrough;
		this->setPixmapPrivate(content, true);
		connect(this->timeLine, &QTimeLine::frameChanged, this, [this](int frame){
			if(this->mapPixmapList.length() <= 1 && this->timeLine->state() != QTimeLine::NotRunning){
				this->timeLine->stop();
			}
			if(frame < this->mapPixmapList.length() && frame >= 0){
				showPixmapNext();
			}
		});
	}

	/**
	 * @brief RpgMapBlock
	 * @param col Int: 列(按照块的宽度倍数)
	 * @param row Int: 行(按照块的高度倍数)
	 * @param tileSetPixmap RpgTileSetBase: 阅读块类对象
	 * @param x Int: 阅读块对象Col
	 * @param y Int: 阅读块对象Row
	 * @param canWalkThrough Bool: 设置是否能走进该区域
	 * @param parentScene: RpgScene: 父场景指针, 显示到哪个场景中
	 * @param parentGraphicsItem: QGraphicsItem: 父GraphicsItem, 继承要用的
	 * @param parent: QObject父类
	 * 注: 这个构造函数有毒
	 */
//	RpgMapBlock(int col, int row,
//				RpgTileSetBase *tileSetPixmap,
//				int x, int y,
//				bool canWalkThrough = true,
//				RpgScene *parentScene = nullptr,
//				QGraphicsItem *parentGraphicsItem = nullptr,
//				QObject *parent = nullptr){
//		if(tileSetPixmap == nullptr){
//			qDebug() << "RpgMapBlock::RpgMapBlock: RpgTileSetBase is a nullptr.";
//			return;
//		}
//		int rows = tileSetPixmap->getRows();
//		int cols = tileSetPixmap->getCols();
//		if(x >= cols){
//			qDebug() << "RpgMapBlock::RpgMapBlock: x out of cols.";
//			return;
//		}
//		if(y >= rows){
//			qDebug() << "RpgMapBlock::RpgMapBlock: y out of rows.";
//			return;
//		}
//		QPixmap pp = tileSetPixmap->getRpgTilePixmap(x, y);
//		qDebug() << "pp:" << pp << CodePath();
//		RpgMapBlock(col, row, pp, canWalkThrough, parentScene, parentGraphicsItem, parent);
//	}

	/**
	 * @brief getCanWalkThrough
	 * @return Bool: 能否通过区域
	 */
	bool getCanWalkThrough(){
		return this->canWalkThrough;
	}

	/**
	 * @brief setLocation
	 * @param point Point: 位置
	 * 设置位置(像素)
	 */
	void setLocation(const QPoint &point){
		this->setPos(point);
	}
	/**
	 * @brief setLocation
	 * @param col
	 * @param row
	 * 设置位置(格子)
	 */
	void setLocation(const int col, const int row){
		this->setPos(col * MapBlockWidth, row * MapBlockHeight);
	}

	/**
	 * @brief addPixmap
	 * @param content
	 * @param index
	 * 添加图片(多个图片的时候要用的)
	 */
	void addPixmap(const QPixmap &content, int index = -1){
		QPixmap *p = new QPixmap(content);
		if(index == -1){
			this->mapPixmapList.append(p);
		}else{
			this->mapPixmapList.insert(index, p);
		}
	}

	void clearPixmap(){
		this->mapPixmapList.clear();
	}

	/**
	 * @brief showPixmapIndex
	 * @param index
	 * 指定现在显示列表中哪一个图片
	 */
	void showPixmapIndex(int index){
		if(index < 0 || index >= this->mapPixmapList.length()){
			qDebug() << "RpgMapBlock::showPixmapIndex: index is out of range. Length:" << this->mapPixmapList.length();
			return;
		}
		this->setPixmap(*this->mapPixmapList.at(index));
	}

	/**
	 * @brief showPixmapNext
	 * 显示下一张图片
	 */
	void showPixmapNext(){
		if(this->mapPixmapList.length() == 1){
			return;
		}
		if(this->currentIndex < 0 || this->currentIndex >= this->mapPixmapList.length()){
			this->currentIndex = 0;
		}
		this->showPixmapIndex(this->currentIndex);
		this->currentIndex++;
	}

	/**
	 * @brief getLength
	 * @return Int: 长度
	 * 获得有几帧
	 */
	int getLength(){
		return this->mapPixmapList.length();
	}

	/**
	 * @brief getLocation
	 * @return Point 位置
	 * 获得所处的位置
	 */
	QPoint getLocation() const{
		return this->pos().toPoint();
	}

	/**
	 * @brief getPixmap
	 * @return Pixmap
	 * 获得当前所显示的Pixmap
	 */
	QPixmap getPixmap() const{
		return this->pixmap();
	}
	/**
	 * @brief setRpgScene
	 * @param scene
	 * 设置显示的Scene
	 */
	void setRpgScene(RpgScene * scene){
		this->parentScene = scene;
	}

	/**
	 * @brief show
	 * 显示到屏幕上
	 */
	void show(){
		if(this->parentScene == nullptr){
			qDebug() << "RpgMapBlock::show(): parentScene is not set.(Null)" << CodePath();
			return;
		}
		if(this->parentScene->itemAt(this->pos(), QTransform()) == this){
			qDebug() << "RpgMapBlock::show(): There has this item in the scene." << CodePath();
			return;
		}
		if(this->pixmap().isNull()){
			qDebug() << "RpgMapBlock::show(): Pixmap is Null." << CodePath();
		}
		this->setZValue(LineZValue(this->location.y()));
		this->parentScene->addItem(this);
		if(this->mapPixmapList.length() >= 1){
			this->timeLine->start();
		}
	}

	/**
	 * @brief hide
	 * 从屏幕移除?
	 */
	void hide(){
		if(this->timeLine->state() == QTimeLine::Running){
			this->timeLine->stop();
		}
		this->hide();
	}

	/**
	 * @brief setDuringTime
	 * @param timeLength
	 * 设置时间长度
	 */
	void setDuringTime(int timeLength){
		if(timeLength <= 0){
			this->timeLine->setDuration(1000);
		}else if(this->mapPixmapList.length() <= 1){
			this->timeLine->setDuration(1000);
		}else{
			this->timeLine->setDuration(timeLength);
		}
	}

	/**
	 * @brief setLayer
	 * @param adjust
	 * 设置当前块所在的层数, 利用zValue来控制层数, adjust的数值根据Global zValue设置.
	 * adjust可以是0-0.9的数, 也可以是0-9的数, 它相当于设置了0-0.9的数.
	 */
	void setLayer(qreal adjust){
		if(adjust >= 0 && adjust < 1){
			this->setZValue(qFloor(this->zValue()) + adjust);
		}else if(adjust >= 1 && adjust < 10){
			this->setZValue(qFloor(this->zValue()) + (adjust / 10.0f));
		}
	}

};

#endif // RPGMAPBLOCK_H
