#ifndef GLOBAL_H
#define GLOBAL_H

#include <QFont>

// 屏幕宽度(像素)
#define ScreenWidth 640
// 屏幕高度(像素)
#define ScreenHeight 480
// 屏幕宽度(像素)(浮点型)
#define ScreenWidthF ScreenWidth##.0f
// 屏幕高度(像素)(浮点型)
#define ScreenHeightF ScreenHeight##.0f

// 一个Block的宽度
#define MapBlockWidth 32
// 一个Block的高度
#define MapBlockHeight 32

#ifndef DEBUG
// 调试模式
#define DEBUG 1
#endif

/**
  * 关于游戏当中的ZValue
  * RPG的设定有
  * 背景: -1 ~ 0 长度:1
  * 内容: 0 ~ 100 INDEX属于0-100, 最大行数100行
  * --> [INDEX + 0] -> [INDEX + 0.3] 地板和地面物, 墙面物
  * --> [INDEX + 0.4] -> 精灵
  * --> [INDEX + 0.5] -> [INDEX + 0.9] 物品
  * 对话框: 100 ~ 101
  * 前置盖幕: 101 ~ 102
  * 道具栏: 102 ~ 103
  * 菜单栏: 110 ~ 111
  */
#define BackgroundZValue -1
#define LineZValue(line) (line)
#define SpiritZValue(line) (line + 0.5)
#define DialogZValue 100
#define FrontZValue 101
#define ItemZValue 102
#define MenuZValue 110

class Global{
public:
	// Dialog的字体
	static QFont dialogFont;
	// 系统的字体
	static QFont applicationFont;
};

#endif // GLOBAL_H
