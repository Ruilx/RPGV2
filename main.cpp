#include <QApplication>

#include <RPG/Global.h>
#include <RPG/About.h>
#include "ExternalFont.h"
#include "Mainw.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// 预置内容
	// --> 字体内容
	ExternalFont fonts;
	if(!fonts.addFont("dialogFont", "./data/fonts/A-OTF-FolkPro-Medium.otf")){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Cannot open the font file: %1").arg("data/fonts/A-OTF-FolkPro-Medium.otf"), QMessageBox::Ok);
		return 1;
	}
	if(!fonts.addFont("applicationFont", "./data/fonts/msyh.ttc")){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Cannot open the font file: %1").arg("data/fonts/msyh.ttc"), QMessageBox::Ok);
		return 1;
	}
	if(!fonts.addFont("simplifyChineseCharactersSupport", "./data/fonts/msyhl.ttc")){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Cannot open the font file: %1").arg("data/fonts/msyhl.ttc"), QMessageBox::Ok);
		return 1;
	}
	QString dialogFontFamilyString = fonts.getDefaultFontIndex("dialogFont");
	if(dialogFontFamilyString.isEmpty()){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Cannot resolve the font file: %1").arg("data/fonts/A-OTF-FolkPro-Medium.otf"), QMessageBox::Ok);
		return 1;
	}
	QString applicationFontFamilyString = fonts.getDefaultFontIndex("applicationFont");
	if(applicationFontFamilyString.isEmpty()){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Cannot resolve the font file: %1").arg("data/fonts/msyh.ttc"), QMessageBox::Ok);
		return 1;
	}
	QString simplifyChineseCharactersSupport = fonts.getDefaultFontIndex("simplifyChineseCharactersSupport");
	if(simplifyChineseCharactersSupport.isEmpty()){
		QMessageBox::critical(nullptr, ApplicationName, QObject::tr("Cannot resolve the font file: %1").arg("data/fonts/msyhl.ttc"), QMessageBox::Ok);
		return 1;
	}
	Global::dialogFont = QFont(dialogFontFamilyString);
	Global::dialogFont.insertSubstitution(Global::dialogFont.family(), simplifyChineseCharactersSupport);
	Global::dialogFont.setPixelSize(22);
	Global::dialogFont.setBold(true);
	Global::applicationFont = QFont(applicationFontFamilyString);
	Global::applicationFont.setBold(false);
	Global::applicationFont.setPixelSize(22);
	About::showWelcomeDialog();

	Mainw w;
	w.show();

	return a.exec();
}
