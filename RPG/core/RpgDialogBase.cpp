#include "RpgDialogBase.h"

int RpgDialogBase::getDialogHeight() const{
	return dialogHeight;
}

void RpgDialogBase::setDialogHeight(int height){
	dialogHeight = height;
	this->rect = QRect(marginH, ScreenHeight - marginV - dialogHeight, ScreenWidth - marginH - marginH, dialogHeight);
}

void RpgDialogBase::setMargin(int h, int v){
	this->marginH = h;
	this->marginV = v;
	this->rect = QRect(marginH, ScreenHeight - marginV - dialogHeight, ScreenWidth - marginH - marginH, dialogHeight);
}

int RpgDialogBase::getMarginH() const {
	return this->marginH;
}

int RpgDialogBase::getMarginV() const {
	return this->marginV;
}

void RpgDialogBase::setPadding(int h, int v){
	this->paddingH = h;
	this->paddingV = v;
	this->selectBarScaleRect = QRect(paddingH, paddingV, ScreenWidth - marginH - marginH - paddingH - paddingH, selectBarHeight);
}

int RpgDialogBase::getPaddingH() const{
	return this->paddingH;
}

int RpgDialogBase::getPaddingV() const{
	return this->paddingV;
}

QImage &RpgDialogBase::getDialogImage(){
	return this->renderedDialogImage;
}

QImage &RpgDialogBase::getSelectBarImage()
{
	return this->renderedSelectBarImage;
}

QImage *RpgDialogBase::getContinueSymbol(int index) const{
	if(index < 0 || index >= 4){
		return nullptr;
	}
	return this->continueSymbol[index];
}

void RpgDialogBase::renderSkin(const QString &skinFilename){
	QFileInfo fInfo(skinFilename);
	qDebug() << "RpgDialogBase::renderSkin: Loading skin file: " << fInfo.absoluteFilePath();
	QImage skin;
	skin.load(skinFilename);
	if(skin.isNull()){
		qDebug() << "RpgDialogBase::renderSkin: Load skin file failed.";
		return;
	}
	if(skin.width() < 192 || skin.height() < 128){
		qDebug() << "RpgDialogBase: RpgDialogBase: Loaded skin not in fit.";
	}
	for(QImage* p: this->backgroundBorder){
		if(p != nullptr){
			delete p;
			p = nullptr;
		}
	}
	for(QImage* p: this->selectBar){
		if(p != nullptr){
			delete p;
			p = nullptr;
		}
	}
	for(QImage* p: this->continueSymbol){
		if(p != nullptr){
			delete p;
			p = nullptr;
		}
	}
	if(this->backgroundOrigin != nullptr){
		delete this->backgroundOrigin;
		this->backgroundOrigin = nullptr;
	}

	QImage background = skin.copy(backgroundRect);
	QImage backgroundBorder = skin.copy(backgroundBorderRect);
	QImage selectBar = skin.copy(selectBarRect);
	QImage continueSymbol = skin.copy(continueSymbolRect);

	this->backgroundOrigin				= new QImage(background);
	this->renderBackground();
	this->backgroundBorder[LeftTop]		= new QImage(backgroundBorder.copy(backgroundBorderLeftTopRect));
	this->backgroundBorder[Top]			= new QImage(backgroundBorder.copy(backgroundBorderTopRect));
	this->backgroundBorder[RightTop]	= new QImage(backgroundBorder.copy(backgroundBorderRightTopRect));
	this->backgroundBorder[Left]		= new QImage(backgroundBorder.copy(backgroundBorderLeftRect));
	this->backgroundBorder[Center]		= new QImage(backgroundBorder.copy(backgroundBorderCenterRect));
	this->backgroundBorder[Right]		= new QImage(backgroundBorder.copy(backgroundBorderRightRect));
	this->backgroundBorder[LeftBottom]	= new QImage(backgroundBorder.copy(backgroundBorderLeftBottomRect));
	this->backgroundBorder[Bottom]		= new QImage(backgroundBorder.copy(backgroundBorderBottomRect));
	this->backgroundBorder[RightBottom]	= new QImage(backgroundBorder.copy(backgroundBorderRightBottomRect));
	this->selectBar[LeftTop]			= new QImage(selectBar.copy(selectBarLeftTopRect));
	this->selectBar[Top]				= new QImage(selectBar.copy(selectBarTopRect));
	this->selectBar[RightTop]			= new QImage(selectBar.copy(selectBarRightTopRect));
	this->selectBar[Left]				= new QImage(selectBar.copy(selectBarLeftRect));
	this->selectBar[Center]				= new QImage(selectBar.copy(selectBarCenterRect));
	this->selectBar[Right]				= new QImage(selectBar.copy(selectBarRightRect));
	this->selectBar[LeftBottom]			= new QImage(selectBar.copy(selectBarLeftBottomRect));
	this->selectBar[Bottom]				= new QImage(selectBar.copy(selectBarBottomRect));
	this->selectBar[RightBottom]		= new QImage(selectBar.copy(selectBarRightBottomRect));
	this->continueSymbol[0]				= new QImage(continueSymbol.copy(continueSymbol1Rect));
	this->continueSymbol[1]				= new QImage(continueSymbol.copy(continueSymbol2Rect));
	this->continueSymbol[2]				= new QImage(continueSymbol.copy(continueSymbol3Rect));
	this->continueSymbol[3]				= new QImage(continueSymbol.copy(continueSymbol4Rect));

	this->renderBackground();
	this->renderDialog();
}

void RpgDialogBase::renderBackground(){
	if(this->background != nullptr){
		delete this->background;
		this->background = nullptr;
	}
	this->background = new QImage(this->rect.width(), this->rect.height(), QImage::Format_ARGB32_Premultiplied);
	this->background->fill(Qt::transparent);
	QImage backgroundCopy = this->backgroundOrigin->scaled(this->rect.width(), this->rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPainter p(this->background);{
		p.setCompositionMode(QPainter::CompositionMode_Source);
		QRectF area(2, 2, background->width() -4, background->height() -4);
		p.drawImage(area, backgroundCopy, area);
		p.end();
	}
}

void RpgDialogBase::renderDialog(){
	this->renderedDialogImage = QImage(this->rect.size(), QImage::Format_ARGB32_Premultiplied);
	this->renderedDialogImage.fill(Qt::transparent);
	QPainter p(&this->renderedDialogImage);{
		int width = this->renderedDialogImage.width();
		int height = this->renderedDialogImage.height();
		p.setCompositionMode(QPainter::CompositionMode_SourceOver);
		// 背景绘图
		p.drawImage(QPoint(0, 0), *this->background);
		// 边框绘图
		for(int i = 16; i < width - 16; i += 16){
			p.drawImage(QRect(i, 0, 16, 16), *this->backgroundBorder[Top]);
			p.drawImage(QRect(i, height - 16, 16, 16), *this->backgroundBorder[Bottom]);
		}
		for(int i = 16; i < height - 16; i += 16){
			p.drawImage(QRect(0, i, 16, 16), *this->backgroundBorder[Left]);
			p.drawImage(QRect(width - 16, i, 16, 16), *this->backgroundBorder[Right]);
		}
		p.drawImage(QRect(0, 0, 16, 16), *this->backgroundBorder[LeftTop]);
		p.drawImage(QRect(width - 16, 0, 16, 16), *this->backgroundBorder[RightTop]);
		p.drawImage(QRect(0, height - 16, 16, 16), *this->backgroundBorder[LeftBottom]);
		p.drawImage(QRect(width - 16, height - 16, 16, 16), *this->backgroundBorder[RightBottom]);
		p.end();
	}

	this->renderedSelectBarImage = QImage(this->selectBarScaleRect.size(), QImage::Format_ARGB32_Premultiplied);
	this->renderedSelectBarImage.fill(Qt::transparent);
	QPainter p2(&this->renderedSelectBarImage);{
		int width = this->renderedSelectBarImage.width();
		int height = this->renderedSelectBarImage.height();
		p2.setCompositionMode(QPainter::CompositionMode_Source);
		// 绘图背景
		for(int i = 0; i < width - 16; i += 16){
			for(int j = 0; j < height - 16; j += 16){
				p2.drawImage(QRect(i, j, 16, 16), *this->selectBar[Center]);
			}
		}
		//边框绘图
		for(int i = 16; i < width - 16; i += 16){
			p2.drawImage(QRect(i, 0, 16, 16), *this->selectBar[Top]);
			p2.drawImage(QRect(i, height - 16, 16, 16), *this->selectBar[Bottom]);
		}
		for(int j = 16; j < height - 16; j += 16){
			p2.drawImage(QRect(0, j, 16, 16), *this->selectBar[Left]);
			p2.drawImage(QRect(width - 16, j, 16, 16), *this->selectBar[Right]);
		}
		int minWidth = qMin(width, 16);
		int minHeight = qMin(height, 16);
		p2.drawImage(QRect(0, 0, 16, 16), *this->selectBar[LeftTop]);
		p2.drawImage(QRect(width - 16 + (16 - minWidth), 0, 16 - (16 - minWidth), 16), *this->selectBar[RightTop]);
		p2.drawImage(QRect(0, height - 16 + (16 - minHeight), 16, 16 - (16 - minHeight)), *this->selectBar[LeftBottom]);
		p2.drawImage(QRect(width - 16 + (16 - minWidth), height - 16 + (16 - minHeight), 16 - (16 - minWidth), 16 - (16 - minHeight)), *this->selectBar[RightBottom]);
		p2.end();
	}
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

void RpgDialogBase::setDialogSkinFilename(const QString &filename){
	this->renderSkin(filename);
}

void RpgDialogBase::setDialogRect(const QRect &rect){
	if(rect.isNull()){
		this->rect = QRect(marginH, ScreenHeight - marginV - dialogHeight, ScreenWidth - marginH - marginH, dialogHeight);
	}else{
		this->rect = rect;
	}
	this->renderBackground();
}

void RpgDialogBase::setSelectBarRect(const QRect &rect){
	if(rect.isNull()){
		this->selectBarScaleRect = QRect(paddingH, paddingV, ScreenWidth - marginH - marginH - paddingH - paddingH, selectBarHeight);
	}else{
		this->selectBarScaleRect = rect;
	}
}
