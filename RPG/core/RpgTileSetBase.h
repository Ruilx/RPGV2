#ifndef RPGTILESETBASE_H
#define RPGTILESETBASE_H

#include <QtCore>
#include <QImage>
#include <QPixmap>
#include <RPG/Global.h>

#include <RPG/exception/RpgNullPointerException.h>
#include <RPG/exception/RpgKeyNotFoundException.h>
/**
 * @brief The RpgTileSetBase class
 * 这个类进行物品/地板/墙面的固定图片读取类
 * 输入一个文件, 将把它分成一堆32x32的小块, 供上屏使用
 * 资源文件在data/images/tilesets
 * 测试: 016-ForestTown02.png
 */
class RpgTileSetBase
{
	int rows = 0;
	int cols = 0;
protected:
	//QList<QImage *> imageList;
	QMap<quint64, QImage *> imageList;

	void readHandle(QString filename){
		qDebug() << "[INFOR] RpgTileSetBase::readHandle(): Reading image path:" << filename;
		QImage fileImage(filename);
		if(fileImage.isNull()){
			return;
		}
		int width = fileImage.width();
		int height = fileImage.height();
		if(width % MapBlockWidth != 0){
			qDebug() << "[WARNI] RpgTileSetBase::readHandle(): Source image's width not grid by" << MapBlockWidth << "px, may cause mapblocks dislocated.";
		}
		if(height % MapBlockHeight != 0){
			qDebug() << "[WARNI] RpgTileSetBase::readHandle(): Source image's height not grid by" << MapBlockHeight <<"px, may cause mapblocks dislocated.";
		}
		this->rows = width / MapBlockWidth;
		this->cols = height / MapBlockHeight;

		for(int i = 0; i < this->rows; i++){
			for(int j = 0; j < this->cols; j++){
				QImage *p = new QImage(fileImage.copy(j * MapBlockHeight, i * MapBlockWidth, MapBlockWidth, MapBlockHeight));
				//this->imageList.append(p);
				this->imageList.insert((quint64)(j) << 32 | (quint64)(i), p);
			}
		}
	}

public:
	RpgTileSetBase(QString filename = QString()){
		if(!filename.isEmpty()){
			this->readHandle(filename);
		}
	}

	QImage *getRpgTile(int x, int y) const {
		return RpgTileSetBase::getRpgTile(QPoint(x, y));
	}

	QImage *getRpgTile(const QPoint &loc) const {
		quint64 index = this->loc2Index(loc);
		if(!this->imageList.contains(index)){
			throw RpgKeyNotFoundException(QString::number(index), "imageList");
			return nullptr;
		}
		QImage *image = this->imageList.value(index, new QImage());
		if(image == nullptr){
			throw RpgNullPointerException(QObject::tr("this->imageList.value(%1)").arg(index));
			return nullptr;
		}
		return image;
	}

	QPixmap getRpgTilePixmap(int x, int y) const {
		QImage *image = getRpgTile(x, y);
		if(image == nullptr){
			throw RpgNullPointerException(QObject::tr("image"));
			return QPixmap();
		}
		return QPixmap::fromImage(*image);
	}

	QPixmap getRpgTilePixmap(QPoint loc) const {
		QImage *image = getRpgTile(loc);
		if(image == nullptr){
			throw RpgNullPointerException(QObject::tr("image"));
			return QPixmap();
		}
		return QPixmap::fromImage(*image);
	}

	int getRows(){ return this->rows; }
	int getCols(){ return this->cols; }

protected:
	quint64 loc2Index(const QPoint &loc) const{
		return (quint64)(loc.x()) << 32 | (quint64)(loc.y());
	}
};

#endif // RPGTILESETBASE_H
