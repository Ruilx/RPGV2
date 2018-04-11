#ifndef RPGDIALOGBASE_H
#define RPGDIALOGBASE_H

#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <QtCore>
#include <QtWidgets>

#define DialogFilename "data/images/skin/blueAlpha.png" //(192x128)

class RpgDialogBase
{
private:
	// 对话框背景原作(128x128)未变形的时候
	QImage *backgroundOrigin = nullptr;
	// 对话框背景变化(根据设置大小)已变形
	QImage *background = nullptr;
	// 对话框边框的9个图标(16x16)
	QImage *backgroundBorder[9] = {nullptr};
	// 选择框边框的9个图标(16x16)
	QImage *selectBar[9] = {nullptr};
	// 继续的小三角形的4个动画(16x16)
	QImage *continueSymbol[4] = {nullptr};

	// 已经做好的对话框背景图片
	QImage renderedDialogImage;
	// 已经做好的选择框的背景图片
	QImage renderedSelectBarImage;

	// 因为对话框默认宽度是全屏宽的, 这是左右和屏幕边的默认间距, 下和屏幕边框的默认间距, 可以被
	// setRect()改变, 可以被setRect(QRect())还原.
	// 左右边距(px)
	int marginH = 10;
	// 上下边距(px)
	int marginV = 10;
	// 对话框高度(px);
	int dialogHeight = 125;
	// 设置结果的对话框大小
	QRect rect = QRect(marginH, ScreenHeight - marginV - dialogHeight, ScreenWidth - marginH - marginH, dialogHeight);

	// 选择框默认宽度是对话框宽度减去两个padding宽, 高度是20px的字符上下各增加5px(30px高),
	// 可以被setSelectBarRect()改变, 可以被setSelectBarRect(QRect())还原
	// 左右间隔(px)
	int paddingH = 10;
	// 上下间隔(px)
	int paddingV = 5;
	// 选择框高度(px)
	int selectBarHeight = 32;
	// 设置结果的选择框大小
	QRect selectBarScaleRect = QRect(paddingH, paddingV, ScreenWidth - marginH - marginH - paddingH - paddingH, selectBarHeight);

	// 各个图片在原图中的位置坐标
	const QRect backgroundRect					= QRect(  0,  0, 128, 128);
	const QRect backgroundBorderRect			= QRect(128,  0,  64,  64);
	const QRect selectBarRect					= QRect(128, 64,  32,  32);
	const QRect continueSymbolRect				= QRect(160, 64,  32,  32);

	const QRect backgroundBorderLeftTopRect		= QRect( 0,  0, 16, 16);
	const QRect backgroundBorderTopRect			= QRect(16,  0, 16, 16);
	const QRect backgroundBorderRightTopRect	= QRect(48,  0, 16, 16);
	const QRect backgroundBorderLeftRect		= QRect( 0, 16, 16, 16);
	const QRect backgroundBorderCenterRect		= QRect(24, 24, 16, 16);
	const QRect backgroundBorderRightRect		= QRect(48, 16, 16, 16);
	const QRect backgroundBorderLeftBottomRect	= QRect( 0, 48, 16, 16);
	const QRect backgroundBorderBottomRect		= QRect(16, 48, 16, 16);
	const QRect backgroundBorderRightBottomRect	= QRect(48, 48, 16, 16);

	const QRect selectBarLeftTopRect			= QRect( 0,  0, 16, 16);
	const QRect selectBarTopRect				= QRect( 8,  0, 16, 16);
	const QRect selectBarRightTopRect			= QRect(16,  0, 16, 16);
	const QRect selectBarLeftRect				= QRect( 0,  8, 16, 16);
	const QRect selectBarCenterRect				= QRect( 8,  8, 16, 16);
	const QRect selectBarRightRect				= QRect(16,  8, 16, 16);
	const QRect selectBarLeftBottomRect			= QRect( 0, 16, 16, 16);
	const QRect selectBarBottomRect				= QRect( 8, 16, 16, 16);
	const QRect selectBarRightBottomRect		= QRect(16, 16, 16, 16);

	const QRect continueSymbol1Rect				= QRect( 0,  0, 16, 16);
	const QRect continueSymbol2Rect				= QRect(16,  0, 16, 16);
	const QRect continueSymbol3Rect				= QRect( 0, 16, 16, 16);
	const QRect continueSymbol4Rect				= QRect(16, 16, 16, 16);

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

	// 重新变换skin文件的时候需要重新载入图像区块
	void renderSkin(const QString &skinFilename);
	// 重新设置背景图片或大小的时候需要重新计算背景图片大小
	void renderBackground();
	// 组合背景图片和边框, 获得最后的图片.(加上边框的图片, 选择框, 继续的小三角形)
	void renderDialog();

public:
	RpgDialogBase(const QString &skinFilename = QString());

	int getDialogHeight() const;
	void setDialogHeight(int height);
	void setMargin(int h, int v);
	int getMarginH() const;
	int getMarginV() const;
	void setPadding(int h, int v);
	int getPaddingH() const;
	int getPaddingV() const;

	QImage &getDialogImage();
	QImage &getSelectBarImage();
	QImage *getContinueSymbol(int index) const;

	QRect getDialogRect() const { return this->rect; }

	QPointF getDialogPosition() const { return QPointF(this->rect.left(), this->rect.top()); }
	QPointF getSelectBarPosition() const { return QPointF(this->selectBarScaleRect.left(), this->selectBarScaleRect.top()); }

	void setSkinFile(const QString &skinFilename){ this->renderSkin(skinFilename); }

protected:
	void setDialogSkinFilename(const QString &filename);
	void setDialogRect(const QRect &rect);
	void setSelectBarRect(const QRect &rect);

};

#endif // RPGDIALOGBASE_H
