#include "RpgDialogBase_.h"



void RpgDialogBase_::renderBackground(){
	this->background = QImage(this->dialogSize, QImage::Format_ARGB32_Premultiplied);
	this->background.fill(Qt::transparent);
	QImage backgroundCopy = this->backgroundOrigin.scaled(this->dialogSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPainter p(this->background);{
		p.setCompositionMode(QPainter::CompositionMode_Source);
		QRectF area(2, 2, background.width() -4, background.height() -4);
		p.drawImage(area, backgroundCopy, area);
		p.end();
	}
}

void RpgDialogBase_::renderDialog(){
	this->renderedDialogImage = QImage(this->dialogSize, QImage::Format_ARGB32_Premultiplied);
	this->renderedDialogImage.fill(Qt::transparent);
	QPainter p(&this->renderedDialogImage);{
		int width = this->renderedDialogImage.width();
		int height = this->renderedDialogImage.height();
		p.setCompositionMode(QPainter::CompositionMode_SourceOver);
		// 背景绘图
		p.drawImage(QPoint(0, 0), this->background);
		// 边框绘图
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

	this->renderedSelectBarImage = QImage(this->selectBarSize, QImage::Format_ARGB32_Premultiplied);
	this->renderedSelectBarImage.fill(Qt::transparent);
	QPainter p2(&this->renderedSelectBarImage);{
		int width = this->renderedSelectBarImage.width();
		int height = this->renderedSelectBarImage.height();
		p2.setCompositionMode(QPainter::CompositionMode_SourceOver);
		// 绘图背景
		for(int i = 0; i < width - 16; i += 16){
			for(int j = 0; j < height - 16; j += 16){
				p2.drawImage(QRect(i, j, 16, 16), this->selectBar[Center]);
			}
		}
		// 边框绘图
		for(int i = 16; i < width - 16; i += 16){
			p2.drawImage(QRect(i, 0, 16, 16), this->selectBar[Top]);
			p2.drawImage(QRect(i, height - 16, 16, 16), this->selectBar[Bottom]);
		}
		for(int j = 16; j < height - 16; j += 16){
			p2.drawImage(QRect(0, j, 16, 16), this->selectBar[Left]);
			p2.drawImage(QRect(width - 16, j, 16, 16), this->selectBar[Right]);
		}
		int minWidth = qMin(width, 16);
		int minHeight = qMin(height, 16);
		p2.drawImage(QRect(0, 0, 16, 16), this->selectBar[LeftTop]);
		p2.drawImage(QRect(width - 16 + (16 - minWidth), 0, 16 - (16 - minWidth), 16), this->selectBar[RightTop]);
		p2.drawImage(QRect(0, height - 16 + (16 - minHeight), 16, 16 - (16 - minHeight)), this->selectBar[LeftBottom]);
		p2.drawImage(QRect(width - 16 + (16 - minWidth), height - 16 + (16 - minHeight), 16 - (16 - minWidth), 16 - (16 - minHeight)), this->selectBar[RightBottom]);
		p2.end();
	}
}

void RpgDialogBase_::renderSkin(const QString &skinFileName){
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
	this->continueSymbol[0]				= continueSymbol.copy(this->continueSymbol1Rect);
	this->continueSymbol[1]				= continueSymbol.copy(this->continueSymbol2Rect);
	this->continueSymbol[2]				= continueSymbol.copy(this->continueSymbol3Rect);
	this->continueSymbol[3]				= continueSymbol.copy(this->continueSymbol4Rect);

	this->renderBackground();
	this->renderDialog();
}

RpgDialogBase_::RpgDialogBase_(const QString &skinFilename){
	QString skin;
	if(skinFilename.isEmpty()){
		skin = DialogFilename;
	}else{
		skin = skinFilename;
	}
	this->renderSkin(skin);
}
