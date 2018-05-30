#ifndef RPGDIALOGHELPER_H
#define RPGDIALOGHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgDialog.h>

class RpgDialogHelper: public QObject
{
	Q_OBJECT

	RpgDialog *rpgDialog = nullptr;
public:
	RpgDialogHelper(RpgDialog *rpgDialog, QObject *parent = nullptr): QObject(parent){
		if(rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog = rpgDialog;
	}

	Q_INVOKABLE void exec(){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->exec();
	}

	Q_INVOKABLE int waitingForDialogComplete(){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return -1;
		}
		return this->rpgDialog->waitingForDialogComplete();
	}

	Q_INVOKABLE void addText(const QString &text){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->addText(text);
	}

	Q_INVOKABLE void addText(const QStringList &textList){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->addText(textList);
	}

	Q_INVOKABLE void setSlowprint(int speed){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->setSlowprint(speed);
	}

	Q_INVOKABLE void setFont(const QFont &font){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->setFont(font);
	}

	Q_INVOKABLE void setTextColor(const QColor &color){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->setTextColor(color);
	}

	Q_INVOKABLE void setDialogWidth(int width){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->setDialogWidth(width);
	}

	Q_INVOKABLE void clearText(){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->clearText();
	}

	Q_INVOKABLE void setCharacterPixmap(const QPixmap &character){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->setCharacterPixmap(character);
	}

	Q_INVOKABLE void setCharacterAround(RpgDialog::AvatarAround avatarAround){
		if(this->rpgDialog == nullptr){
			qDebug() << CodePath() << "RpgDialog is nullptr.";
			return;
		}
		this->rpgDialog->setCharacterAround(avatarAround);
	}
};

#endif // RPGDIALOGHELPER_H
