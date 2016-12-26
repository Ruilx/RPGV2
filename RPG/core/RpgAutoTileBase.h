#ifndef RPGAUTOTILEBASE_H
#define RPGAUTOTILEBASE_H

#include <QtCore>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <QtWidgets>

#define AutoTileImageBlockWidth 96
#define AutoTileImageBlockHeight 128

typedef struct Block_t{
	QImage singleBlockImage;
	QImage *outCornerImage[4];
	QImage *blockImage[9];
	~Block_t(){
		for(int i = 0; i < 4; i++){
			if(outCornerImage[i] != nullptr){
				delete outCornerImage[i];
			}
		}
		for(int i = 0; i < 9; i++){
			if(blockImage[i] != nullptr){
				delete blockImage[i];
			}
		}
	}
}Block;

class RpgAutoTileBase
{
private:
	QString name;
	QImage originImage;

	QList<Block> blockImageList;

	const QRect singleBlockRect = QRect(0, 0, 32, 32);
	const QRect backgroundRect = QRect(32, 0, 32, 32);
	const QRect rightBottomOutCornerRect = QRect(64, 0, 16, 16);
	const QRect leftBottomOutCornerRect = QRect(80, 0, 16, 16);
	const QRect rightTopOutCornerRect = QRect(64, 16, 16, 16);
	const QRect leftTopOutCornerRect = QRect(80, 16, 16, 16);

	const QPoint rightBottomOutCornerLocalOffset = QPoint(0, 0);
	const QPoint leftBottomOutCornerLocalOffset = QPoint(16, 0);
	const QPoint rightTopOutCornerLocalOffset = QPoint(0, 16);
	const QPoint leftTopOutCornerLocalOffset = QPoint(16, 16);

	const QRect leftTopRect = QRect(0, 32, 32, 32);
	const QRect topRect = QRect(32, 32, 32, 32);
	const QRect rightTopRect = QRect(64, 32, 32, 32);
	const QRect leftRect = QRect(0, 64, 32, 32);
	const QRect centerRect = QRect(32, 64, 32, 32);
	const QRect rightRect = QRect(64, 64, 32, 32);
	const QRect leftBottomRect = QRect(0, 96, 32, 32);
	const QRect bottomRect = QRect(32, 96, 32, 32);
	const QRect rightBottomRect = QRect(64, 96, 32, 32);
public:
	/**
	 * @brief The SplitBlock enum
	 * 在组合图像中九宫格图片中指定图片表示位置的顺序
	 */
	enum SplitBlock{
		LeftTop = 0,		// 左上角 ↖
		Top = 1,			// 上边  ↑
		RightTop = 2,		// 右上角 ↗
		Left = 3,			// 左边  ←
		Center = 4,			// 中间  ·
		Right = 5,			// 右边  →
		LeftBottom = 6,		// 左下角 ↙
		Bottom = 7,			// 下边  ↓
		RightBottom = 8		// 右下角 ↘
	};

	enum SplitCorner{
		RightBottomCorner = 0,
		LeftBottomCorner = 1,
		RightTopCorner = 2,
		LeftTopCorner = 3
	};
private:
	QRect adjustRect(QRect original, int offset = 0){
		return QRect(original.left() * (offset + 1), original.top(), original.width(), original.height());
	}

	void renderBlock(const QString &autoTileFileName){
		QImage blockImage;
		blockImage.load(autoTileFileName);
		if(blockImage.isNull()){
			qDebug() << "RpgAutoTileBase::renderBlock: load block texture failed.";
			return;
		}
		if(blockImage.height() < AutoTileImageBlockHeight || blockImage.width() < AutoTileImageBlockWidth){
			qDebug() << "RpgAutoTileBase::renderBlock: load block texture is not fit.";
			return;
		}
		int count = blockImage.width() / AutoTileImageBlockWidth;
		for(int i = 0; i < count; i++){
			Block block;
			block.singleBlockImage = blockImage.copy(this->adjustRect(this->singleBlockRect, i));

			QImage background = blockImage.copy(this->adjustRect(this->backgroundRect, i));
			QImage backgroundOld = background;
			QPainter p(&background);{
				p.setCompositionMode(QPainter::CompositionMode_SourceOver);
				p.drawImage(this->rightBottomOutCornerLocalOffset, blockImage.copy(this->rightBottomOutCornerRect));
				p.end();
			}
			block.outCornerImage[RightBottomCorner] = new QImage(background);
			background = backgroundOld;

			QPainter p2(&background);{
				p2.setCompositionMode(QPainter::CompositionMode_SourceOver);
				p2.drawImage(this->leftBottomOutCornerLocalOffset, blockImage.copy(this->leftBottomOutCornerRect));
				p2.end();
			}
			block.outCornerImage[LeftBottomCorner] = new QImage(background);
			background = backgroundOld;

			QPainter p3(&background);{
				p3.setCompositionMode(QPainter::CompositionMode_SourceOver);
				p3.drawImage(this->rightTopOutCornerLocalOffset, blockImage.copy(this->rightTopOutCornerRect));
				p3.end();
			}
			block.outCornerImage[RightTopCorner] = new QImage(background);
			background = backgroundOld;

			QPainter p4(&background);{
				p4.setCompositionMode(QPainter::CompositionMode_SourceOver);
				p4.drawImage(this->leftBottomOutCornerLocalOffset, blockImage.copy(this->leftTopOutCornerRect));
				p4.end();
			}
			block.outCornerImage[LeftBottomCorner] = new QImage(background);

			block.blockImage[LeftTop] = new QImage(blockImage.copy(this->leftTopRect));
			block.blockImage[Top] = new QImage(blockImage.copy(this->topRect));
			block.blockImage[RightTop] = new QImage(blockImage.copy(this->rightTopRect));
			block.blockImage[Left] = new QImage(blockImage.copy(this->leftRect));
			block.blockImage[Center] = new QImage(blockImage.copy(this->centerRect));
			block.blockImage[Right] = new QImage(blockImage.copy(this->rightRect));
			block.blockImage[LeftBottom] = new QImage(blockImage.copy(this->leftBottomRect));
			block.blockImage[Bottom] = new QImage(blockImage.copy(this->bottomRect));
			block.blockImage[RightBottom] = new QImage(blockImage.copy(this->rightBottomRect));

			this->blockImageList.append(block);
		}
	}

public:
	// 构造函数
	RpgAutoTileBase(const QString &autoTileFileName = QString(), const QString &name = QString()){
		if(!name.isEmpty()){
			this->name = name;
		}
		if(!autoTileFileName.isEmpty()){
			this->renderBlock(autoTileFileName);
		}
	}

	// 设置地图块名
	void setName(const QString &name){
		this->name = name;
	}

	// 得到地图块名
	QString getName() const{
		return this->name;
	}

	// 设置变化图块名
	void setAutoTileFileName(const QString &filename){
		this->blockImageList.clear();
		this->renderBlock(filename);
	}

	// 得到1x1地图块的画面
	QImage getSingleBlockImage(int index = 0){
		if(index < 0 || index >= this->blockImageList.length()){
			qDebug() << "RpgAutoTileBase::getSingleBlockImage(): index is out of range.";
			if(index == 0){
				qDebug() << "RpgAutoTileBase::getSingleBlockImage(): blockImageList is empty.";
			}
			return QImage();
		}
		return this->blockImageList.at(index).singleBlockImage;
	}

	// 得到边界地图块的画面
	QImage getOutCornerImage(SplitCorner corner, int index = 0){
		if(index < 0 || index >= this->blockImageList.length()){
			qDebug() << "RpgAutoTileBase::getSingleBlockImage(): index is out of range.";
			if(index == 0){
				qDebug() << "RpgAutoTileBase::getSingleBlockImage(): blockImageList is empty.";
			}
			return QImage();
		}
		if(this->blockImageList.at(index).outCornerImage[corner] != nullptr){
			return QImage(*this->blockImageList.at(index).outCornerImage[corner]);
		}else{
			qDebug() << "RpgAutoTileBase::getSingleBlockImage(): blockImageList[index].outCornerImage[corner] is nullptr.";
			return QImage();
		}
	}

	// 得到地图块的画面
	QImage getBlockImage(SplitBlock block, int index = 0){
		if(index < 0 || index >= this->blockImageList.length()){
			qDebug() << "RpgAutoTileBase::getBlockImage(): index is out of range.";
			if(index == 0){
				qDebug() << "RpgAutoTileBase::getBlockImage(): blockImageList is empty.";
			}
			return QImage();
		}
		if(this->blockImageList.at(index).blockImage[block] != nullptr){
			return QImage(*this->blockImageList.at(index).blockImage[block]);
		}else{
			qDebug() << "RpgAutoTileBase::getBlockImage(): blockImageList[index].blockImage[block] is nullptr.";
			return QImage();
		}
	}
};

#endif // RPGAUTOTILEBASE_H
