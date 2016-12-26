#include "Mainw.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Mainw w;
	w.show();

	return a.exec();
}
