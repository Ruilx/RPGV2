#ifndef RPGDIALOGBASE__H
#define RPGDIALOGBASE__H

#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <QtCore>
#include <QtWidgets>

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
 */
class RpgDialogBase_{

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
	QImage continueSymbol[4];

	// 已经做好的背景图片
	QImage renderedDialogImage;

	// 已经做好的选择框图片
	QImage renderedSelectBarImage


public:
	RpgDialogBase_()
	{

	}
};

#endif // RPGDIALOGBASE__H
