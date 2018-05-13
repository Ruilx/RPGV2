#ifndef GLOBAL_H
#define GLOBAL_H

#include <QFont>

// 屏幕宽度(像素)
#define ScreenWidth 640
// 屏幕高度(像素)
#define ScreenHeight 480
// 屏幕宽度(像素)(浮点型)
#define ScreenWidthF 640.0f
// 屏幕高度(像素)(浮点型)
#define ScreenHeightF 480.0f

// 一个Block的宽度
#define MapBlockWidth 32
// 一个Block的高度
#define MapBlockHeight 32

#ifndef QT_DEBUG
// 调试模式
#define QT_DEBUG 1
#endif

/**
  * 关于游戏当中的ZValue
  * RPG的设定有
  * 背景: -1 ~ 0 长度:1
  * 内容: 0 ~ 100 INDEX属于0-100, 最大行数100行
  * --> [INDEX + 0] -> [INDEX + 0.3] 地板和地面物, 墙面物
  * --> [INDEX + 0.4] -> 精灵
  * --> [INDEX + 0.5] -> [INDEX + 0.9] 物品(高于主角平均身高的物品)
  * 对话框: 100 ~ 101
  * 前置盖幕: 101 ~ 102
  * 道具栏: 102 ~ 103
  * 菜单栏: 110 ~ 111
  */
#define BackgroundZValue -1				//背景图层
#define LineZValue(line) (line)			//行图层
#define SpiritZValue(line) (line + 0.4)	//精灵图层
#define ThingZValue(line) (line + 0.5)	//物品图层
#define DialogZValue 100				//对话框图层
#define FrontZValue 101					//前端(迷雾)图层
#define ItemZValue 102					//道具图层
#define MenuZValue 110					//菜单图层
#define TopZValue 1000					//最顶上的图层

class Global{
public:
	// Dialog的字体
	static QFont dialogFont;
	// 系统的字体
	static QFont applicationFont;
};

#ifdef QT_DEBUG
#	define CodePath() "[" __FILE__ ":" % QString::number(__LINE__) % "]" % __PRETTY_FUNCTION__ % ": "
#else
#	define CodePath() ""
#endif

#endif // GLOBAL_H
