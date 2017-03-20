#ifndef GLOBAL_H
#define GLOBAL_H

#define ScreenWidth 640
#define ScreenHeight 480
#define ScreenWidthF ScreenWidth##.0f
#define ScreenHeightF ScreenHeight##.0f

#define MapBlockWidth 32
#define MapBlockHeight 32

#ifndef DEBUG
#define DEBUG 1
#endif

/**
  * 关于游戏当中的ZValue
  * RPG的设定有
  * 背景: -1 ~ 0 长度:1
  * 内容: 0 ~ 100 INDEX属于0-100, 最大行数100行
  * --> [INDEX]: 第INDEX行, 每行有5层, 分别是Index, Index+0.1, Index+0.2, Index+0.3, Index+0.4
  * --> [INDEX+0.5]: 当前行所在的精灵
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


#endif // GLOBAL_H
