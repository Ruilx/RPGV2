#include <QApplication>

#include <QFontDatabase>
#include <RPG/Global.h>
#include <RPG/About.h>
#include "Mainw.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// 预置内容
	// --> 字体内容
	int dialogFontId = QFontDatabase::addApplicationFont("./data/fonts/A-OTF-FolkPro-Medium.otf");
	if(dialogFontId == -1){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Fatal Error: Cannot access the font file."), QMessageBox::Ok);
#ifdef DEBUG
		QMessageBox::information(nullptr, ApplicationName, QObject::tr("Debug Info: Font file: ./data/fonts/A-OTF-FolkPro-Medium.otf"), QMessageBox::Ok);
#endif
		return 1;
	}
	QStringList dialogFontList(QFontDatabase::applicationFontFamilies(dialogFontId));
	if(dialogFontList.isEmpty()){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Fatal Error: Cannot resolve the font file."), QMessageBox::Ok);
#ifdef DEBUG
		QMessageBox::information(nullptr, ApplicationName, QObject::tr("Debug Info: Font file: ./data/fonts/A-OTF-FolkPro-Medium.otf"), QMessageBox::Ok);
#endif
		return 1;
	}
	Global::dialogFont = QFont(dialogFontList.at(0));
	Global::dialogFont.insertSubstitution(Global::dialogFont.family(), "Microsoft YaHei Light");
	Global::dialogFont.setPixelSize(22);
	Global::dialogFont.setBold(true);
	//Global::dialogFont.setPixelSize(26);

	int applicationFontId = QFontDatabase::addApplicationFont("./data/fonts/msyh.ttc");
	if(applicationFontId == -1){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Fatal Error: Cannot access the font file."), QMessageBox::Ok);
#ifdef DEBUG
		QMessageBox::information(nullptr, ApplicationName, QObject::tr("Debug Info: Font file: ./data/fonts/msyh.ttc"), QMessageBox::Ok);
#endif
		return 1;
	}
	QStringList applicationFontList(QFontDatabase::applicationFontFamilies(applicationFontId));
	if(applicationFontList.isEmpty()){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Fatal Error: Cannot resolve the font file."), QMessageBox::Ok);
#ifdef DEBUG
		QMessageBox::information(nullptr, ApplicationName, QObject::tr("Debug Info: Font file: ./data/fonts/msyh.ttc"), QMessageBox::Ok);
#endif
		return 1;
	}
	Global::applicationFont = QFont(applicationFontList.at(0));;
	Global::applicationFont.setBold(false);
	Global::applicationFont.setPixelSize(22);

	Mainw w;
	w.show();

	return a.exec();
}
