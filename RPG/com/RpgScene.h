#ifndef RPGSCENE_H
#define RPGSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>
#include <RPG/com/RpgDialog.h>
#include <RPG/com/RpgBanner.h>
#include <RPG/com/RpgChoice.h>
#include <RPG/com/RpgItem.h>
/**
 * @brief The RpgScene class
 * RPGScene类是RPG游戏中的场景类, 其本质是一个QGraphicsScene, 在scene上增加需要的内容
 * 每一个Scene配备一个RPGDialog, 一个RPGSelectDialog, 一个RPGItemDialog, 一个RPGSystemDialog
 * 每一个组件在一个Scene只能出现一次, Scene中存储着地图的地图块, 精灵等
 *
 * --> RpgDialog类不再是单例模式, 而是以Scene返回方式获取.
 * --> RpgChoice类不再是单例模式, 而是以Scene返回方式获取.
 */
class RpgItem;
class RpgScene : public QGraphicsScene
{
	Q_OBJECT
	RpgDialog *dialog = nullptr;
	RpgBanner *banner = nullptr;
	RpgChoice *choice = nullptr;
public:
	RpgScene(QObject *parent = nullptr) : QGraphicsScene(parent){
		this->setScenePos(0.0f, 0.0f);
		this->dialog = new RpgDialog(this, this);
		this->banner = new RpgBanner(this, this);
		this->choice = new RpgChoice(this, this);
	}

	void setSceneRect(const QRectF &rect){ QGraphicsScene::setSceneRect(rect);}
	inline void setSceneRect(qreal x, qreal y, qreal w, qreal h) { this->setSceneRect(QRectF(x, y, w, h)); }
	inline void setScenePos(const QPointF &point){ this->setSceneRect(point.x(), point.y(), ScreenWidthF, ScreenHeightF); }
	inline void setScenePos(qreal left, qreal top){ this->setScenePos(QPointF(left, top)); }

	QRectF getSceneRect() const{ return this->sceneRect(); }
	QPointF getScenePos() const{ return this->sceneRect().topLeft(); } // 应该写成ViewPos好一点, 但是看上面都写成ScenePos了...闲了改过来
	QSizeF getSceneSize() const{ return this->sceneRect().size(); }

	RpgDialog *getRpgDialog() const{ Q_ASSERT_X(this->dialog, "RpgDialog Nulled", "Dialog not found!"); return this->dialog; }
	RpgBanner *getRpgBanner() const{ Q_ASSERT_X(this->banner, "RpgBanner Nulled", "Banner not found!"); return this->banner; }
	RpgChoice *getRpgChoise() const{ Q_ASSERT_X(this->choice, "RpgChoice Nulled", "Choice not found!"); return this->choice; }

	bool addRpgItem(RpgItem *item);
	bool removeRpgItem(RpgItem *item);
};

#endif // RPGSCENE_H
