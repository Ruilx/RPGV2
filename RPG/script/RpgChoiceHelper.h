#ifndef RPGCHOICEHELPER_H
#define RPGCHOICEHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgChoice.h>

class RpgChoiceHelper: public QObject
{
	Q_OBJECT

	RpgChoice *rpgChoice = nullptr;
public:
	RpgChoiceHelper(RpgChoice *rpgChoice, QObject *parent = nullptr): QObject(parent){
		if(rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice = rpgChoice;
	}

	Q_INVOKABLE void exec(){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->exec();
	}

	Q_INVOKABLE void waitingForChooseComplete(int second = -1){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->waitingForChooseComplete(second);
	}

	Q_INVOKABLE int getChose(){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return -1;
		}
		return this->rpgChoice->getChose();
	}

	Q_INVOKABLE void addChoiceText(const QString &text){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->addChoiceText(text);
	}

	Q_INVOKABLE void setChoiceEnabled(int choiceIndex, bool enabled){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->setChoiceEnabled(choiceIndex, enabled);
	}

	Q_INVOKABLE void setSlowprint(int speed){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->setSlowprint(speed);
	}

	Q_INVOKABLE void setFont(const QFont &font){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->setFont(font);
	}

	Q_INVOKABLE void setTextColor(const QColor &color){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->setTextColor(color);
	}

	Q_INVOKABLE void setDialogWidth(int width){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->setDialogWidth(width);
	}

	Q_INVOKABLE void clearChoiceText(){
		if(this->rpgChoice == nullptr){
			qDebug() << CodePath() << "RpgChoice nullptr.";
			return;
		}
		this->rpgChoice->clearChoiceText();
	}
};

#endif // RPGCHOICEHELPER_H
