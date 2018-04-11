#ifndef EXTERNALFONT_H
#define EXTERNALFONT_H

#include <RPG/Global.h>
#include <QHash>
#include <QString>
#include <QFontDatabase>

class ExternalFont
{
	QHash<QString, int> fontMap;
public:
	ExternalFont();

	bool addFont(const QString &name, const QString &fileName);
	QString getDefaultFontIndex(QString name);
	QStringList getFontList(QString name) const;
};

#endif // EXTERNALFONT_H
