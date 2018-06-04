#ifndef RPGDIALOGBASE_H
#define RPGDIALOGBASE_H

#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <QtCore>
#include <QtWidgets>

#define DialogFilename "data/images/skin/blackAlpha.png" //(192x128)

/**
 * @brief The RpgDialogBase_ class
 * 重写加载对话框类
 *
 * 只提供图片, 可以设置大小, 根据大小渲染相应的图片.
 * 设置项目:
 * --> 基准图片
 * --> 对话框大小
 * ----> 对话框宽
 * ----> 对话框高
 * --> 对话框padding
 * ----> 对话框H padding
 * ----> 对话框V padding
 * --> 选择框大小
 * ----> 选择框宽
 * ----> 选择框高
 * 给定项目:
 * --> 对话框背景图片
 * --> 选择框背景图片
 * --> 三角形背景图片
 * 2018/04/28 在获取Pixmap的时候才进行渲染
 * --- 更换记录: 考虑到在开始的时候就去渲染的话会出现改变一次皮肤或大小都会重新渲染一次
 * --- 就会出现频繁内存读写而调用者并不想要某些渲染的结果导致无效率计算.
 * 2018/04/28 返回换成QPixmap
 * --- 同步Item的Pixmap, 不需要每次都转换一次..
 *
 */
class RpgDialogBase
{
protected:
	// 预定的对话框大小是屏幕整体宽度减去两个marginH, 高度固定125.
	const int MarginH = 10;
	const int MarginV = 10;
	const int Height = 125;

	// 预定的选择框大小宽度是对话框宽度减去两个paddingH,
	// 高度预定为32, 可以根据字体高度进行二次设定
	const int PaddingH = 10;
	const int PaddingV = 5;
	const int SelectBarHeight = 32;

private:
	// 对话框背景原作(128x128)未变形的时候
	QImage backgroundOrigin;
	// 对话框背景成品(已变形)
	QImage background;
	// 对话框的9个图标
	QImage backgroundBorder[9];
	// 选择框边框的9个图标
	QImage selectBar[9];
	// 继续的小三角图标
	QPixmap continueSymbol[4];

	QSize dialogSize = QSize(ScreenWidth - MarginH - MarginH, Height);
	QSize selectBarSize = QSize(ScreenWidth - MarginH - MarginH - PaddingH - PaddingH, SelectBarHeight);

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

	// 被调用的背景处理
	const QPixmap renderBackground();
	// 被调用的对话框的皮肤(会自动调用renderBackground函数)
	const QPixmap renderDialog();
	// 被调用的选项框的皮肤
	const QPixmap renderSelectBar();
	// 重新加载皮肤文件的时候进行处理和图片重绘
	void renderSkin(const QString &skinFileName);

public:
	RpgDialogBase(const QString &skinFilename = QString());

	inline const QSize getDialogSize() const { return this->dialogSize; }
	inline const QSize getSelectBarSize() const { return this->selectBarSize; }
	inline const QSize getContinueSymbolSize() const { return QSize(16, 16); }
	void setDialogSize(const QSize &size){
		this->dialogSize = size;
		this->selectBarSize = QSize(size.width() - PaddingH - PaddingH, this->selectBarSize.height());
	}
	void setSelectBarSize(const QSize &size){ this->selectBarSize = size; }

	const QPixmap getDialogImage(){ return this->renderDialog(); }
	const QPixmap getSelectBarImage(){ return this->renderSelectBar(); }
	const QPixmap getContinueSymbolImage(int index) {
		if(index < 0 || index >= 4){
			return QPixmap();
		}else{
			return this->continueSymbol[index];
		}
	}

	void loadDialogSkin(const QString &filename){ this->renderSkin(filename); }


};

#endif // RPGDIALOGBASE_H
