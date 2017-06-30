#ifndef RPGSCENE_H
#define RPGSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>
#include <RPG/com/RpgDialog.h>
#include <RPG/com/RpgBanner.h>
/**
 * @brief The RpgScene class
 * RPGScene类是RPG游戏中的场景类, 其本质是一个QGraphicsScene, 在scene上增加需要的内容
 * 每一个Scene配备一个RPGDialog, 一个RPGSelectDialog, 一个RPGItemDialog, 一个RPGSystemDialog
 * 每一个组件在一个Scene只能出现一次, Scene中存储着地图的地图块, 精灵等
 *
 * --> RpgDialog类不再是单例模式, 而是以Scene返回方式获取.
 */
class RpgScene : public QGraphicsScene
{
	RpgDialog *dialog = new RpgDialog(this, this);
	RpgBanner *banner = new RpgBanner(this, this);
public:
	RpgScene(QObject *parent = nullptr) : QGraphicsScene(parent){

	}

	void setSceneRect(const QRectF &rect){
		QGraphicsScene::setSceneRect(rect);
		//this->dialog->setViewportOffset(QPointF(rect.left(), rect.top()));
	}
	inline void setSceneRect(qreal x, qreal y, qreal w, qreal h) { this->setSceneRect(QRectF(x, y, w, h)); }
	void setScenePos(const QPointF &point){ this->setSceneRect(point.x(), point.y(), ScreenWidthF, ScreenHeightF); }
	inline void setScenePos(qreal left, qreal top){ this->setScenePos(QPointF(left, top)); }

	RpgDialog *getRpgDialog(){ return this->dialog; }
	RpgBanner *getRpgBanner(){ return this->banner; }


};

#endif // RPGSCENE_H
