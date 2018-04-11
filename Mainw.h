#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>
#include <RPG/com/RpgWidget.h>

class Mainw : public QMainWindow
{
	Q_OBJECT
	RpgWidget *w = new RpgWidget(this);

	void closeEvent(QCloseEvent *event){
		if(!w->getCanBeClose()){
			int result = QMessageBox::question(this, About::appName(), tr("Do you really want to quit the game?\nThe progress not saved will be lost."), QMessageBox::Yes | QMessageBox::Cancel);
			if(result != QMessageBox::Yes){
				event->ignore();
				return;
			}
		}
		event->accept();
		qApp->exit();
		return;
	}
public:
	Mainw(QWidget *parent = 0);
	~Mainw();
};

#endif // MAINW_H
