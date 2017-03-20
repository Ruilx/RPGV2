#ifndef RPGMAPBLOCK_H
#define RPGMAPBLOCK_H

#include <QtCore>
#include <QGraphicsPixmapItem>
#include <RPG/Global.h>

/**
 * @brief The RpgMapBlock class
 * 地图上的块, 单独的形成一个背景, 一般是作为背景的物件, 窗口, 窗帘, 柜子, 桌子和其他的物品的东西
 * 如果物块大于1x1, 将会被分解成1x1的块, 由于游戏中每一行都需要设置不同的ZValue, 保证上一层的玩家精灵
 * 会被下一层的物件挡住.
 * RpgMapBlock是一个场景放置块类, 输入一个Location, 内容, 是否让精灵通过, parent(为什么要parent?)
 * 内容可以为空, 如果是那一种一段时间变化的的动图, 可以使用addPixmap添加帧
 * 现在正在想怎么让能动的背景的运动方式timer是主类统一发信号还是自我timer对象进行控制?
 * (一个QTimeLine和一堆QTimeLine的问题)
 */
class RpgMapBlock: public QGraphicsPixmapItem
{
	/**
	 * @brief mapPixmapList
	 * 这个就是存储的图像序列
	 */
	QList<QPixmap> mapPixmapList;
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
			this->setPixmap(content);
			if(append == true){
				this->addPixmap(content);
			}
		}
	}

protected:
	/**
	 * @brief setPixmap
	 * @param content
	 * 继承要用的? 或者构造函数调用用的
	 */
	void setPixmap(const QPixmap &content){
		this->setPixmapPrivate(content);
	}

public:
	/**
	 * @brief RpgMapBlock
	 * @param location Point: 位置
	 * @param content Pixmap: 内容
	 * @param canWalkThrough Bool: 设置能否走进该区域
	 * @param parent: 父类(作为一个QGraphicsItem自带的)
	 */
	RpgMapBlock(const QPoint &location, const QPixmap &content = QPixmap(), bool canWalkThrough = true, QGraphicsItem *parent = nullptr): QGraphicsPixmapItem(parent){
		this->setPos(location);
		this->canWalkThrough = canWalkThrough;
		this->setPixmapPrivate(content, true);
	}

	/**
	 * @brief RpgMapBlock
	 * @param col Int: 列(按照块的宽度倍数)
	 * @param row Int: 行(按照块的宽度倍数)
	 * @param content Pixmap: 内容
	 * @param canWalkThrough Bool: 设置能否走进该区域
	 * @param parent: 父类(作为一个QGraphicsItem自带的)
	 */
	RpgMapBlock(int col, int row, const QPixmap &content = QPixmap(), bool canWalkThrough = true, QGraphicsItem *parent = nullptr): QGraphicsPixmapItem(parent){
		this->setPos(col * MapBlockWidth, row * MapBlockHeight);
		this->canWalkThrough = canWalkThrough;
		this->setPixmapPrivate(content, true);
	}

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
	 * 设置位置
	 */
	void setLocation(const QPoint &point){
		this->setPos(point);
	}
	/**
	 * @brief setLocation
	 * @param col
	 * @param row
	 * 设置位置
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
		if(index == -1){
			this->mapPixmapList.append(content);
		}else{
			this->mapPixmapList.insert(index, content);
		}
	}

	/**
	 * @brief showPixmapIndex
	 * @param index
	 * 指定现在显示列表中哪一个图片
	 */
	void showPixmapIndex(int index){
		if(index < 0 || index >= this->mapPixmapList.length()){
			qDebug() << "RpgMapBlock::showPixmapIndex: index is out of range";
			return;
		}
		this->setPixmap(this->mapPixmapList.at(index));
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
};

#endif // RPGMAPBLOCK_H
