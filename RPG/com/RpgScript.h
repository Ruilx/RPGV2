#ifndef RPGSCRIPT_H
#define RPGSCRIPT_H

#include <QtCore>
#include <QtWidgets>
#include <QJSEngine>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgFileManager.h>

/**
 * @brief The RpgScript class
 * RPGScript类
 * 是执行RPG脚本的解析模块
 * 脚本使用JSEngine进行运行
 */

class RpgScript : public QObject
{
	Q_OBJECT

	QJSEngine *engine = new QJSEngine(this);
	QHash<QString, QJSValue> modules;
	QString scriptFileName;
public:
	explicit RpgScript(QObject *parent = nullptr) : QObject(parent){
		this->engine->installExtensions(QJSEngine::AllExtensions);
		this->modules.clear();
	}

//	void setScriptName(const QString &fileName){
//		if(fileName.isEmpty()){
//			qDebug() << CodePath() << "Script file name is empty.";
//			return;
//		}
//		if(!QFile::exists(fileName)){
//			qDebug() << CodePath() << "Script file not exist.";
//			return;
//		}
//		this->scriptFileName = fileName;
//	}
	/**
	 * @brief setScriptName
	 * @param name
	 * 设置可执行的脚本, 名称由RpgFileManager提供
	 */
	void setScriptName(const QString &name){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::ScriptFile, name);
		if(filename.isEmpty()){
			qDebug() << CodePath() << "Script name:" << name << "not found in file manager.";
			return;
		}
		this->scriptFileName = filename;
	}

	QString exec(){
		if(this->scriptFileName.isEmpty()){
			qDebug() << CodePath() << "Script file name is empty.";
			return QString("exit");
		}
		QStringList keys = this->modules.keys();
		for(const QString &key: keys){
			this->engine->globalObject().setProperty(key, this->modules.value(key));
		}
		QFile f(this->scriptFileName);
		if(!f.open(QIODevice::ReadOnly)){
			qDebug() << CodePath() << "Cannot open the script file:" << f.fileName();
			return QString("exit");
		}
		QString script = f.readAll();
		if(f.isOpen()){
			f.close();
		}

		QJSValue res = this->engine->evaluate(script, this->scriptFileName);
		if(res.isError()){
			qDebug() << CodePath() << "Uncaught exception at line" << res.property("lineNumber").toInt()
					 << "Error:" << res.toString();
			return QString("exit");
		}
		if(!res.isString()){
			qDebug() << CodePath() << "Main script returns not a string to tell main thread which scene will be loaded, Aborted.";
			QString type = "unknown";
			if(res.isArray()){
				type = "array";
			}else if(res.isBool()){
				type = "bool";
			}else if(res.isCallable()){
				type = "function";
			}else if(res.isDate()){
				type = "date";
			}else if(res.isNull()){
				type = "null";
			}else if(res.isNumber()){
				type = "number";
			}else if(res.isObject()){
				type = "object";
#if QT_VERSION > 0x050800
			}else if(res.isQMetaObject()){
				type = "meta";
#endif
			}else if(res.isQObject()){
				type = "qobject";
			}else if(res.isRegExp()){
				type = "regexp";
			}else if(res.isString()){
				type = "string";
			}else if(res.isUndefined()){
				type = "undefined";
			}else if(res.isVariant()){
				type = "varient";
			}
			qDebug() << CodePath() << "Main result type:" << type << "value:" << res.toString();
			return QString("exit");
		}
		return res.toString();
	}

	void addJsValue(const QString &name, QObject *obj){
		if(name.isEmpty()){
			qDebug() << CodePath() << "Name is empty.";
			return;
		}
		if(obj == nullptr){
			qDebug() << CodePath() << "Object is nullptr";
			return;
		}
		this->modules.insert(name, this->engine->newQObject(obj));
	}

	QJSValue getModule(const QString &name){
		return this->modules.value(name);
	}

	void removeModule(const QString &name){
		this->modules.remove(name);
	}


signals:

public slots:
};

#endif // RPGSCRIPT_H
