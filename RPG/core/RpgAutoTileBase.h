#ifndef RPGAUTOTILEBASE_H
#define RPGAUTOTILEBASE_H

#include <QtCore>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <QtWidgets>

#define AutoTileImageBlockWidth 96
#define AutoTileImageBlockHeight 128

class Block{
//	QImage *singleBlockImage;
//	/* ┌┐
//	 * └┘
//	 */
//	QImage *singleLineImage[2][3];
//	/* ┌─ ── ─┐ ┌┐ ││ ││
//	 * └─ ── ─┘ ││ ││ └┘
//	 */
//	QImage *singleCornerImage[4];
//	/* ┌─ ─┐ │└ ┘│
//	 * │┌ ┐│ └─ ─┘
//	 */
//	QImage *singleTeeImage[4];
//	/* ┘└ ┘│ ── │└
//	 * ── ┐│ ┐┌ │┌
//	 */
//	QImage *singleHubImage;
//	/* ┘└
//	 * ┐┌
//	 */
//	QImage *multiCornerImage[2][4];
//	/*        └ ┘  ┌─ ─┐ │   │
//	 *  ┌ ┐        │   │ └─ ─┘
//	 */
//	QImage *multiBorderImage[4];
//	/* ── │      │
//	 *    │  ──  │
//	 */
//	QImage *multiTeeImage[2][4];
//	/* ── ── ┘   └ │└ │  ┘│  │
//	 * ┐   ┌ ── ── │  │┌  │ ┐│
//	 */
//	QImage *multiAreaImage[4];
//	/* ┘└ ┘      └
//	 *    ┐  ┐┌  ┌
//	 */
//	QImage *multiHubImage[4];
//	/* ┘└ ┘└ ┘   └
//	 * ┐   ┌ ┐┌ ┐┌
//	 */
//	QImage *oppositeCornerImage[2];
//	/* ┘   └
//	 *  ┌ ┐
//	 */
//	QImage *fillImage;
//	/*
//	 *
//	 */
	/**
	 * @brief imageMap
	 * QMap Image from unsigned int8 to QPixmap
	 * Key: quint8 Note:
	 *       Bit7 Bit6     Bit5  Bit4        Bit3   Bit2       Bit1 Bit0
	 * 0b       x    x        x     x           x      x          x    x -> x=0 pass, x=1 block
	 *    leftTop  top rightTop right rightBottom bottom leftBottom left
	 * Note that if 'top' 'left' 'right' 'bottom' blocked (set to 1), the rest
	 * area will force to set with table below:
	 *              top left right bottom
	 *  topLeft       1    1     x      x
	 *  topRight      1    x     1      x
	 *  bottomLeft    x    1     x      1
	 *  bottomRight   x    x     1      1
	 *  [ x = no effect, 1 = force to set 1 ]
	 */
	QMap<quint8, QPixmap> imageMap;
public:
	/*
	 * The key(quint8) will be one of below:
	 */
	enum BlockType{
		None						= 0x00,
		LeftBottomMultiInnerCorner	= 0x02,
		RightBottomMultiInnerCorner	= 0x08,
		MultiAreaBottom				= 0x0A,
		BottomMulti					= 0x0E,
		RightTopMultiInnerCorner	= 0x20,
		MultiAreaSlash				= 0x22,
		MultiAreaRight				= 0x28,
		MultiHubRightBottom			= 0x2A,
		BottomMultiTeeRight			= 0x2E,
		RightMulti					= 0x38,
		RightMultiTeeBottom			= 0x3A,
		RightBottomMultiCorner		= 0x3E,
		LeftTopMultiInnerCorner		= 0x80,
		MultiAreaLeft				= 0x82,
		LeftMulti					= 0x83,
		MultiAreaBackSlash			= 0x88,
		MultiHubLeftBottom			= 0x8A,
		LeftMultiTeeBottom			= 0x8B,
		BottomMultiTeeLeft			= 0x8E,
		LeftBottomMultiCorner		= 0x8F,
		MultiAreaTop				= 0xA0,
		MultiHubLeftTop				= 0xA2,
		LeftMultiTeeTop				= 0xA3,
		MultiHubRightTop			= 0xA8,
		SingleHub					= 0xAA,
		LeftSingleTee				= 0xAB,
		BottomSingleTee				= 0xAE,
		LeftBottomSingleCorner		= 0xAF,
		RightMultiTeeTop			= 0xB8,
		RightSingleTee				= 0xBA,
		VerticalPassedSingleLine	= 0xBB,
		RightBottomSingleCorner		= 0xBE,
		TopPassedSingleLine			= 0xBF,
		TopMulti					= 0xE0,
		TopMultiTeeLeft				= 0xE2,
		LeftTopMultiCorner			= 0xE3,
		TopMultiTeeRight			= 0xE8,
		TopSingleTee				= 0xEA,
		LeftTopSingleCorner			= 0xEB,
		HorizonalPassedSingleLine	= 0xEE,
		RightPassedSingleLine		= 0xEF,
		RightTopMultiCorner			= 0xF8,
		RightTopSingleCorner		= 0xFA,
		BottomPassedSingleLine		= 0xFB,
		LeftPassedSingleLine		= 0xFE,
		ClosedFrame					= 0xFF
	};

	enum BlockLocation{
		Block_LeftTop     = 0b10000000,
		Block_RightTop    = 0b00100000,
		Block_RightBottom = 0b00001000,
		Block_LeftBottom  = 0b00000010,
		Block_Top         = 0b01000000 | Block_LeftTop  | Block_RightTop,
		Block_Right       = 0b00010000 | Block_RightTop | Block_RightBottom,
		Block_Bottom      = 0b00000100 | Block_LeftBottom | Block_RightBottom,
		Block_Left        = 0b00000001 | Block_LeftTop  | Block_LeftBottom,

		Block_LT = Block_LeftTop,
		Block_T  = Block_Top,
		Block_RT = Block_RightTop,
		Block_R  = Block_Right,
		Block_RB = Block_RightBottom,
		Block_B  = Block_Bottom,
		Block_LB = Block_LeftBottom,
		Block_L  = Block_Left,
	};

	~Block(){
		this->imageMap.clear();
	}

	Block(){
		this->imageMap.clear();
	}

	void insertImage(BlockType type, const QImage &image){
		QPixmap i = QPixmap::fromImage(image);
		if(i.isNull()){
			qDebug() << CodePath() << "Image is null.";
			return;
		}
		this->imageMap.insert(type, i);
		qDebug() << CodePath() << "Image information:" << image;
	}

	void insertImage(BlockType type, const QPixmap &pixmap){
		this->imageMap.insert(type, pixmap);
	}

	QPixmap getImage(BlockLocation location){
		return this->imageMap.value(location, QPixmap());
	}

	int getCount() const{
		return this->imageMap.count();
	}

	void _dumpAllImages(const QString &dirpath = QString())const {
		QString _dirPath = dirpath;
		if(dirpath.isEmpty()){
			_dirPath = QDir::currentPath();
		}
		QDir dir(_dirPath);
		if(!dir.exists()){
			QDir::current().mkpath(_dirPath);
		}
		int count = 0;
		for(QMap<quint8, QPixmap>::ConstIterator i = this->imageMap.constBegin(); i != this->imageMap.constEnd(); i++){
			if(!i.value().isNull()){
				i.value().save(dir.path() % "/0x" % QString::number(i.key(), 16) % ".png");
				count ++;
			}
		}
		qDebug() << CodePath() << "Successfully dumped" << count << "/ 47 block image(s) at:" << dir.path();
	}
};

class RpgAutoTileBase
{
private:
	QString name;
	QImage originImage;

	QList<Block> blockImageList;

	const QRect singleBlockRect = QRect(0, 0, 32, 32);				// ClosedFrame
	const QRect backgroundRect = QRect(32, 0, 32, 32);				// Unknown
	const QRect singleHubRect = QRect(64, 0, 32, 32);				// SingleHub
	const QRect rightBottomInnerCornerRect = QRect(64, 0, 16, 16);	// RightBottomMultiInnerCorner
	const QRect leftBottomInnerCornerRect = QRect(80, 0, 16, 16);		// LeftBottomMultiInnerCorner
	const QRect rightTopInnerCornerRect = QRect(64, 16, 16, 16);		// RightTopMultiInnerCorner
	const QRect leftTopInnerCornerRect = QRect(80, 16, 16, 16);		// LeftTopMultiInnerCorner

	const QPoint rightBottomInnerCornerLocalOffset = QPoint(0, 0);
	const QPoint leftBottomInnerCornerLocalOffset = QPoint(16, 0);
	const QPoint rightTopInnerCornerLocalOffset = QPoint(0, 16);
	const QPoint leftTopInnerCornerLocalOffset = QPoint(16, 16);

	const QRect leftTopRect = QRect(0, 32, 32, 32);					// LeftTopMultiCorner
	const QRect topRect = QRect(32, 32, 32, 32);					// TopMulti
	const QRect rightTopRect = QRect(64, 32, 32, 32);				// RightTopMultiCorner
	const QRect leftRect = QRect(0, 64, 32, 32);					// LeftMulti
	const QRect centerRect = QRect(32, 64, 32, 32);					// None
	const QRect rightRect = QRect(64, 64, 32, 32);					// RightMulti
	const QRect leftBottomRect = QRect(0, 96, 32, 32);				// LeftBottomMultiCorner
	const QRect bottomRect = QRect(32, 96, 32, 32);					// BottomMulti
	const QRect rightBottomRect = QRect(64, 96, 32, 32);			// RightBottomMultiCorner

	const QRect leftTopHHalfRect = QRect(0, 32, 32, 16);			// leftTop32 Top		// Maybe unused. replaced by leftTop
	const QRect leftBottomHHalfRect = QRect(0, 112, 32, 16);		// leftBottom32 Bottom
	const QRect rightTopHHalfRect = QRect(64, 32, 32, 16);			// rightTop32 Top		// Maybe unused. replaced by rightTop
	const QRect rightBottomHHalfRect = QRect(64, 112, 32, 16);		// rightBottom32 Bottom
	const QRect topHHalfRect = QRect(32, 32, 32, 16);				// top32 Top			// Maybe unused. replaced by top
	const QRect bottomHHalfRect = QRect(32, 112, 32, 16);			// bottom32 Bottom

	const QRect leftTopVHalfRect = QRect(0, 32, 16, 32);			// leftTop32 Left		// Maybe unused. replaced by leftTop
	const QRect leftBottomVHalfRect = QRect(0, 96, 16, 32);			// leftBottom32 Left
	const QRect rightTopVHalfRect = QRect(64, 32, 16, 32);			// rightTop32 Right		// Maybe unused. replaced by rightTop
	const QRect rightBottomVHalfRect = QRect(64, 96, 16, 32);		// rightBottom32 Right
	const QRect leftVHalfRect = QRect(0, 64, 16, 32);				// left32 Left			// Maybe unused. replaced by left
	const QRect rightVHalfRect = QRect(80, 64, 16, 32);				// right32 Right

	const QPoint leftTopHHalfLocalOffset = QPoint(0, 0);
	const QPoint leftBottomHHalfLocalOffset = QPoint(0, 16);
	const QPoint rightTopHHalfLocalOffset = QPoint(0, 0);
	const QPoint rightBottomHHalfLocalOffset = QPoint(16, 0);
	const QPoint topHHalfLocalOffset = QPoint(0, 0);
	const QPoint bottomHHalfLocalOffset = QPoint(0, 16);

	const QPoint leftTopVHalfLocalOffset = QPoint(0, 0);
	const QPoint leftBottomVHalfLocalOffset = QPoint(0, 0);
	const QPoint rightTopVHalfLocalOffset = QPoint(16, 0);
	const QPoint rightBottomVHalfLocalOffset = QPoint(16, 0);
	const QPoint leftHHalfLocalOffset = QPoint(0, 0);
	const QPoint rightHHalfLocalOffset = QPoint(16, 0);

	const QPoint leftTopPos = QPoint(0, 0);
	const QPoint leftBottomPos = QPoint(0, 16);
	const QPoint rightTopPos = QPoint(16, 0);
	const QPoint rightBottomPos = QPoint(16, 16);

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
	inline QRect adjustRect(QRect original, int offset = 0)const {
		return QRect(original.left() * (offset + 1), original.top(), original.width(), original.height());
	}

	void renderBlock(const QString &autoTileFileName);

public:
	// 构造函数
	RpgAutoTileBase(const QString &autoTileFileName = QString(), const QString &name = QString()){
		if(!name.isEmpty()){
			this->name = name;
		}
		if(!autoTileFileName.isEmpty()){
			this->renderBlock(autoTileFileName);
			this->blockImageList.at(0).getCount();
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

	// Debug
	void _dumpImage(int index = 0){
		if((this->blockImageList.length() > index)){
			this->blockImageList.at(index)._dumpAllImages();
		}else{
			qDebug() << CodePath() << "Index is out of range.";
		}

	}

//	// 得到1x1地图块的画面
//	QImage getSingleBlockImage(int index = 0){
//		if(index < 0 || index >= this->blockImageList.length()){
//			qDebug() << "RpgAutoTileBase::getSingleBlockImage(): index is out of range.";
//			if(index == 0){
//				qDebug() << "RpgAutoTileBase::getSingleBlockImage(): blockImageList is empty.";
//			}
//			return QImage();
//		}
//		return this->blockImageList.at(index).getImage(Block::ClosedFrame);
//	}

//	// 得到边界地图块的画面
//	QImage getOutCornerImage(SplitCorner corner, int index = 0){
//		if(index < 0 || index >= this->blockImageList.length()){
//			qDebug() << "RpgAutoTileBase::getSingleBlockImage(): index is out of range.";
//			if(index == 0){
//				qDebug() << "RpgAutoTileBase::getSingleBlockImage(): blockImageList is empty.";
//			}
//			return QImage();
//		}
//		if(this->blockImageList.at(index).outCornerImage[corner] != nullptr){
//			return QImage(*this->blockImageList.at(index).outCornerImage[corner]);
//		}else{
//			qDebug() << "RpgAutoTileBase::getSingleBlockImage(): blockImageList[index].outCornerImage[corner] is nullptr.";
//			return QImage();
//		}
//	}

//	// 得到地图块的画面
//	QImage getBlockImage(SplitBlock block, int index = 0){
//		if(index < 0 || index >= this->blockImageList.length()){
//			qDebug() << "RpgAutoTileBase::getBlockImage(): index is out of range.";
//			if(index == 0){
//				qDebug() << "RpgAutoTileBase::getBlockImage(): blockImageList is empty.";
//			}
//			return QImage();
//		}
//		if(this->blockImageList.at(index).blockImage[block] != nullptr){
//			return QImage(*this->blockImageList.at(index).blockImage[block]);
//		}else{
//			qDebug() << "RpgAutoTileBase::getBlockImage(): blockImageList[index].blockImage[block] is nullptr.";
//			return QImage();
//		}
//	}

};

#endif // RPGAUTOTILEBASE_H
