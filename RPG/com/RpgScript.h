#ifndef RPGSCRIPT_H
#define RPGSCRIPT_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>

/**
 * @brief The RpgScript class
 * RPGScript类
 * 是执行RPG脚本的解析模块
 * 脚本按行存储, 每一行一个指令. 拿到指令后, 传至widget的ready函数进行操作
 */

class RpgScript : public QObject
{
	Q_OBJECT
public:
	explicit RpgScript(QObject *parent = nullptr);

signals:

public slots:
};

#endif // RPGSCRIPT_H

/*
[
{
	"name": "init",
	"func":[
		{
			"oper": "banner",
			"args": {
				"start_opacity": 0.0,
				"end_opacity": 1.0,
				"foreground_pixmap": "data\image\title.png",
				"background_color": "#FFFFFF",
				"speed": 1000,
			}
		},
		{
			"oper": "sleep",
			"args": {
				"sleep": 1000,
			}
		},
		{
			"oper": "banner",
			"args": {
				"start_opacity": 1.0,
				"end_opacity": 0.0,
			}
		},
		{
			"oper": "sleep",
			"args": {
				"sleep": 1000,
			}
		}
	]
},
{
	"name": "map",
	"scene": "title",
	"loc_x": 0,
	"loc_y": 0,
	"trigger": "press", # press, cross, touch
	"premise": [
		{
			"name": "var_check",
			"var": "is_that_opened",
			"op": "=" # = > >= < <= !=
			"val": 0,
		}
	],
	"func": [
		{
			"oper": "value",
			"args": {
				"name": "is_that_opened",
				"val": 1,
			}
		},
		{
			"oper": "dialog",
			"args": {
				"avatar": "Jack",
				"speed": 1000,
				"text": [
					"没有人能够预见到未来",
					"但是你, 也许是因为上帝眷顾你, 便施舍给你你的未来",
					"并教你改变它",
					"但是现实的残忍让你无论如何努力也看不到预见的未来有一丝变化",
					"你也不能自暴自弃",
					"因为上帝一直在看着你在成长."
				],
			}
		}
	]

}
]
 */
