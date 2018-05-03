#include "RpgDialogBase.h"

const QPixmap RpgDialogBase::renderBackground(){
	QPixmap background = QPixmap(this->dialogSize);
	background.fill(Qt::transparent);
	QPixmap backgroundCopy = QPixmap::fromImage(this->backgroundOrigin.scaled(this->dialogSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	QPainter p(&background);{
		p.setCompositionMode(QPainter::CompositionMode_SourceOver);
		QRectF area(2, 2, background.width() -4, background.height() -4);
		p.drawPixmap(area, backgroundCopy, area);
		p.end();
	}
	return background;
}

const QPixmap RpgDialogBase::renderDialog(){
	QPixmap background = this->renderBackground();
	QPainter p(&background);{
		int width = this->dialogSize.width();
		int height = this->dialogSize.height();
		p.setCompositionMode(QPainter::CompositionMode_SourceOver);
		for(int i = 16; i < width - 16; i += 16){
			p.drawImage(QRect(i, 0, 16, 16), this->backgroundBorder[Top]);
			p.drawImage(QRect(i, height - 16, 16, 16), this->backgroundBorder[Bottom]);
		}
		for(int i = 16; i < height - 16; i += 16){
			p.drawImage(QRect(0, i, 16, 16), this->backgroundBorder[Left]);
			p.drawImage(QRect(width - 16, i, 16, 16), this->backgroundBorder[Right]);
		}
		p.drawImage(QRect(0, 0, 16, 16), this->backgroundBorder[LeftTop]);
		p.drawImage(QRect(width - 16, 0, 16, 16), this->backgroundBorder[RightTop]);
		p.drawImage(QRect(0, height - 16, 16, 16), this->backgroundBorder[LeftBottom]);
		p.drawImage(QRect(width - 16, height - 16, 16, 16), this->backgroundBorder[RightBottom]);
		p.end();
	}
	return background;
}

const QPixmap RpgDialogBase::renderSelectBar(){
	QPixmap selectBarImage = QPixmap(this->selectBarSize);
	selectBarImage.fill(Qt::transparent);
	QPainter p(&selectBarImage);{
		int width = this->selectBarSize.width();
		int height = this->selectBarSize.height();
		p.setCompositionMode(QPainter::CompositionMode_Source);
		// 背景绘图
		for(int i = 0; i < width - 16; i += 16){
			for(int j = 0; j < height - 16; j += 16){
				p.drawImage(QRect(i, j, 16, 16), this->selectBar[Center]);
			}
		}
		// 边框绘图
		for(int i = 16; i < width - 16; i += 16){
			p.drawImage(QRect(i, 0, 16, 16), this->selectBar[Top]);
			p.drawImage(QRect(i, height - 16, 16, 16), this->selectBar[Bottom]);
		}
		for(int j = 16; j < height - 16; j += 16){
			p.drawImage(QRect(0, j, 16, 16), this->selectBar[Left]);
			p.drawImage(QRect(width - 16, j, 16, 16), this->selectBar[Right]);
		}
		int minWidth = qMin(width, 16);
		int minHeight = qMin(height, 16);
		p.drawImage(QRect(0, 0, 16, 16), this->selectBar[LeftTop]);
		p.drawImage(QRect(width - 16 + (16 - minWidth), 0, 16 - (16 - minWidth), 16), this->selectBar[RightTop]);
		p.drawImage(QRect(0, height - 16 + (16 - minHeight), 16, 16 - (16 - minHeight)), this->selectBar[LeftBottom]);
		p.drawImage(QRect(width - 16 + (16 - minWidth), height - 16 + (16 - minHeight), 16 - (16 - minWidth), 16 - (16 - minHeight)), this->selectBar[RightBottom]);
		p.end();
	}
	return selectBarImage;
}

void RpgDialogBase::renderSkin(const QString &skinFileName){
	QFileInfo fInfo(skinFileName);
	qDebug() << CodePath() << ": Loading skin file:" << fInfo.absoluteFilePath();
	QImage skin;
	skin.load(skinFileName);
	if(skin.isNull()){
		qDebug() << CodePath() << ": Load skin file failed.";
		return;
	}
	if(skin.width() < 192 || skin.height() < 128){
		qDebug() << CodePath() << ": Loaded skin is not fit.";
	}

	QImage background = skin.copy(this->backgroundRect);
	QImage backgroundBorder = skin.copy(this->backgroundBorderRect);
	QImage selectBar = skin.copy(this->selectBarRect);
	QImage continueSymbol = skin.copy(this->continueSymbolRect);

	this->backgroundOrigin				= background;
	this->backgroundBorder[LeftTop]		= backgroundBorder.copy(this->backgroundBorderLeftTopRect);
	this->backgroundBorder[Top]			= backgroundBorder.copy(this->backgroundBorderTopRect);
	this->backgroundBorder[RightTop]	= backgroundBorder.copy(this->backgroundBorderRightTopRect);
	this->backgroundBorder[Left]		= backgroundBorder.copy(this->backgroundBorderLeftRect);
	this->backgroundBorder[Center]		= backgroundBorder.copy(this->backgroundBorderCenterRect);
	this->backgroundBorder[Right]		= backgroundBorder.copy(this->backgroundBorderRightRect);
	this->backgroundBorder[LeftBottom]	= backgroundBorder.copy(this->backgroundBorderLeftBottomRect);
	this->backgroundBorder[Bottom]		= backgroundBorder.copy(this->backgroundBorderBottomRect);
	this->backgroundBorder[RightBottom]	= backgroundBorder.copy(this->backgroundBorderRightBottomRect);
	this->selectBar[LeftTop]			= selectBar.copy(this->selectBarLeftTopRect);
	this->selectBar[Top]				= selectBar.copy(this->selectBarTopRect);
	this->selectBar[RightTop]			= selectBar.copy(this->selectBarRightTopRect);
	this->selectBar[Left]				= selectBar.copy(this->selectBarLeftRect);
	this->selectBar[Center]				= selectBar.copy(this->selectBarCenterRect);
	this->selectBar[Right]				= selectBar.copy(this->selectBarRightRect);
	this->selectBar[LeftBottom]			= selectBar.copy(this->selectBarLeftBottomRect);
	this->selectBar[Bottom]				= selectBar.copy(this->selectBarBottomRect);
	this->selectBar[RightBottom]		= selectBar.copy(this->selectBarRightBottomRect);
	this->continueSymbol[0]				= QPixmap::fromImage(continueSymbol.copy(this->continueSymbol1Rect));
	this->continueSymbol[1]				= QPixmap::fromImage(continueSymbol.copy(this->continueSymbol2Rect));
	this->continueSymbol[2]				= QPixmap::fromImage(continueSymbol.copy(this->continueSymbol3Rect));
	this->continueSymbol[3]				= QPixmap::fromImage(continueSymbol.copy(this->continueSymbol4Rect));
}

RpgDialogBase::RpgDialogBase(const QString &skinFilename){
	QString skin;
	if(skinFilename.isEmpty()){
		skin = DialogFilename;
	}else{
		skin = skinFilename;
	}
	this->renderSkin(skin);
}
