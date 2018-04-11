#include "ExternalFont.h"

ExternalFont::ExternalFont(){}

bool ExternalFont::addFont(const QString &name, const QString &fileName){
	int fontId = QFontDatabase::addApplicationFont(fileName);
	if(fontId == -1){
		return false;
	}
	this->fontMap.insert(name, fontId);
	return true;
}

QString ExternalFont::getDefaultFontIndex(QString name){
	if(!this->fontMap.contains(name)){
		return QString();
	}
	QStringList fontList = QFontDatabase::applicationFontFamilies(this->fontMap.value(name));
	if(fontList.length() >= 1){
		return fontList.at(0);
	}else{
		return QString();
	}
}

QStringList ExternalFont::getFontList(QString name) const{
	if(!this->fontMap.contains(name)){
		return QStringList();
	}
	return QFontDatabase::applicationFontFamilies(this->fontMap.value(name));
}
