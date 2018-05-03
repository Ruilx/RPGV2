#ifndef RPGSCRIPT_H
#define RPGSCRIPT_H

#include <QtCore>
#include <QtWidgets>
#include <QJSEngine>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>

/**
 * @brief The RpgScript class
 * RPGScript类
 * 是执行RPG脚本的解析模块
 * 脚本使用JSEngine进行运行
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
