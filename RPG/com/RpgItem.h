#ifndef RPGITEM_H
#define RPGITEM_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/About.h>
#include <RPG/Global.h>

/**
 * @brief The RpgItem class
 * RPGItem类
 * 可以让脚本流程内容自建一个内容组件, 并传递至对应的Scene进行显示
 * 可新建, 并定义Pixmap内容
 */
class RpgItem : public QGraphicsPixmapItem
{
public:
	enum ItemPosition{
		Pos_Absolute = 0,
		Pos_Relative = 1,
	};
private:
//	QGraphicsPixmapItem *item = new QGraphicsPixmapItem(nullptr);
	ItemPosition itemPosition = Pos_Relative;
	QPointF _pos = QPointF(0, 0);
public:
	explicit RpgItem(QGraphicsItem *parentItem = nullptr): QGraphicsPixmapItem(parentItem){
		//this->hide();
	}

//	~RpgItem(){
//		QList<QGraphicsItem*> childItems = this->childItems();
//		if(!childItems.isEmpty()){
//			qDebug() << CodePath() << "This item has children items, be careful with children items had built.";
//		}
//		if(this->item->scene() != nullptr){
//			this->item->scene()->removeItem(this->item);
//		}
//		if(item != nullptr){
//			delete this->item;
//		}
//	}

	~RpgItem(){
//		QGraphicsPixmapItem::~QGraphicsPixmapItem();
		/* ~QGraphicsItem()
		 * Destroys the QGraphicsItem and all its children.
		 * If this item is currently associated with a scene,
		 * the item will be removed from the scene before it is deleted.
		 */
	}

//	void setPixmap(const QPixmap &pixmap){
//		if(pixmap.isNull()){
//			qDebug() << CodePath() << "Given pixmap is not a valid pixmap.";
//			return;
//		}
//		this->setPixmap(pixmap);
//	}

	void setPixmap(const QPixmap &pixmap){
		QGraphicsPixmapItem::setPixmap(pixmap);
	}

	void setPixmap(const QString &pixmapFileName){
		if(pixmapFileName.isEmpty() || !QFile::exists(pixmapFileName)){
			qDebug() << CodePath() << "Pixmap file name is not valid or empty, cannot load the pixmap.";
			return;
		}
		QPixmap p = QPixmap(pixmapFileName);
		if(p.isNull()){
			qDebug() << CodePath() << "Pixmap file cannot be loaded as pixmap.";
			return;
		}
		QGraphicsPixmapItem::setPixmap(p);
	}

	void setPos(const QPointF &pos){
		this->_pos = pos;
	}

	QPointF getPos() const{
		return this->_pos;
	}

	QPointF pos() const{
		return this->getPos();
	}

	void setRealPos(const QPointF &pos){
		QGraphicsPixmapItem::setPos(pos);
	}

	QPointF getRealPos() const{
		return QGraphicsPixmapItem::pos();
	}

	void setItemPosition(ItemPosition itemPosition){
		this->itemPosition = itemPosition;
	}

	ItemPosition getItemPosition(){
		return this->itemPosition;
	}

	bool isNull() const{
		return this->pixmap().isNull();
	}

//	void setScene(RpgScene *parentScene){
//		if(parentScene == nullptr){
//			qDebug() << CodePath() << "Giving parent scene is not valid.";
//			return;
//		}
//		if(this->item->parentItem() != nullptr){
//			qDebug() << CodePath() << "Cannot add a item it already had a parent item, please add the 'parent' item directly!";
//			return;
//		}
//		if(this->item->pixmap().isNull()){
//			qDebug() << CodePath() << "Cannot add a null pixmap to the scene, item's has no contents can be show!";
//			return;
//		}
//		switch(this->itemPosition){
//			case Pos_Absolute:
//				this->item->setPos(this->pos);
//				break;
//			case Pos_Relative:
//				this->item->setPos(parentScene->sceneRect().topLeft() + this->pos);
//				break;
//		}
//		parentScene->addItem(this->item);
//		emit this->addedToScene(parentScene);
//	}

//	void leaveScene(){
//		RpgScene *parentScene = qobject_cast<RpgScene*>(this->item->scene());
//		if(parentScene == nullptr){
//			qDebug() << CodePath() << "Item has no parent scene yet, cannot leave parent scene.";
//			return;
//		}
//		parentScene->removeItem(this->item);
//		emit this->leaveFromScene(parentScene);
//	}
};

#endif // RPGITEM_H
