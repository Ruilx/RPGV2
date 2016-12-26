#ifndef RPGMAPBLOCK_H
#define RPGMAPBLOCK_H

#include <QtCore>
#include <QGraphicsPixmapItem>
#include <RPG/Global.h>

class RpgMapBlock: public QGraphicsPixmapItem
{
	QList<QPixmap> mapPixmapList;
	int currentIndex = 0;
	bool canWalkThrough = true;

	void setPixmapPrivate(const QPixmap &content, bool append = false){
		if(!content.isNull()){
			if(content.width() != MapBlockWidth || content.height() != MapBlockHeight){
				qDebug() << QObject::tr("RpgMapBlock::setPixmapPrivate(): content is not fit at (%1, %2), source: (%1, %2)").arg(MapBlockWidth).arg(MapBlockHeight).arg(content.width()).arg(content.height());
			}
			this->setPixmap(content);
			if(append == true){
				this->addPixmap(content);
			}
		}
	}

protected:
	void setPixmap(const QPixmap &content){
		this->setPixmapPrivate(content);
	}

public:
	RpgMapBlock(const QPoint &location, const QPixmap &content = QPixmap(), bool canWalkThrough = true, QGraphicsItem *parent = nullptr): QGraphicsPixmapItem(parent){
		this->setPos(location);
		this->canWalkThrough = canWalkThrough;
		this->setPixmapPrivate(content, true);
	}

	RpgMapBlock(int col, int row, const QPixmap &content = QPixmap(), bool canWalkThrough = true, QGraphicsItem *parent = nullptr): QGraphicsPixmapItem(parent){
		this->setPos(col * MapBlockWidth, row * MapBlockHeight);
		this->canWalkThrough = canWalkThrough;
		this->setPixmapPrivate(content, true);
	}

	bool getCanWalkThrough(){
		return this->canWalkThrough;
	}

	void setLocation(const QPoint &point){
		this->setPos(point);
	}

	void addPixmap(const QPixmap &content, int index = -1){
		if(index == -1){
			this->mapPixmapList.append(content);
		}else{
			this->mapPixmapList.insert(index, content);
		}
	}

	void showPixmapIndex(int index){
		if(index < 0 || index >= this->mapPixmapList.length()){
			qDebug() << "RpgMapBlock::showPixmapIndex: index is out of range";
			return;
		}
		this->setPixmap(this->mapPixmapList.at(index));
	}

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

	int getLength(){
		return this->mapPixmapList.length();
	}

	QPoint getLocation() const{
		return this->pos().toPoint();
	}

	QPixmap getPixmap() const{
		return this->pixmap();
	}
};

#endif // RPGMAPBLOCK_H
