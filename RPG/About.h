#ifndef ABOUT_H
#define ABOUT_H

#if 1
#include <QtCore>
#include <QApplication>
#include <QMessageBox>

#define ApplicationName QObject::tr("VRPGV2")
#define OrganizationName "GT-Soft Studio"
#define AuthorName "Ruilx"
#define AuthorEmail "zrrabbit@gmail.com"
#define AuxiliaryName ""
#define AuxiliaryEmail ""
#define ApplicationBugReportEmail "zrrabbit@gmail.com"
#define WebPage "http://www.Ruilx.pw"
//#define UsingLicense "GNU General Public License"
#define UsingLicense "Private"
#define UsingLicenseVersion 0x00000000
#define UsingSDK "Qt"
#define UsingSDKVersion (QT_VERSION)
#define ApplicationVersion 0x0D000100
//                            | | | +-Revised Version (Patch)
//                            | | +---Minor Version
//                            | +-----Major Version
//                            +-------Edition:
//  0x00 = Stable
//  0x01 = Standard
//  0x0A = Alpha
//  0x0B = Beta
//  0x0C = Commercial (OpenSource)
//  0x0D = Develop
//  0x0E = Enhance
//  0x0F = Final
//  0x10 = Release
//  0x11 = RC (Release Candidate)
//#define Version ApplicationVersion
#define ApplicationLanguage 0x1033

class About{
	QMap<int, QString> versionEditionList;
public:
	About(){
		this->versionEditionList.clear();
		this->versionEditionList.insert(0x00, ""); // Stable will not to output any modifier string.
		this->versionEditionList.insert(0x01, "Standard");
		this->versionEditionList.insert(0x0A, "Alpha");
		this->versionEditionList.insert(0x0B, "Beta");
		this->versionEditionList.insert(0x0C, "Commercial");
		this->versionEditionList.insert(0x0D, "Developing");
		this->versionEditionList.insert(0x0E, "Enhance");
		this->versionEditionList.insert(0x0F, "Final");
		this->versionEditionList.insert(0x10, "Release");
		this->versionEditionList.insert(0x11, "RC");
		this->versionEditionList.insert(0xFF, "Unknown");
	}

	QString getVersionEditionInTheMap(int version)/* const*/{
		return this->versionEditionList.value(version, QString());
	}

//	static const QString applicationName = ApplicationName;
//	static const QString organizationName = OrganizationName;
//	static const QString authorName = AuthorName;
//	static const QString authorEmail = AuthorEmail;
//	static const QString auxiliaryName = AuxiliaryName;
//	static const QString auxiliaryEmail = AuxiliaryEmail;
//	static const QString applicationBugReportEmail = ApplicationBugReportEmail;
//	static const QString webPage = WebPage;
//	static const QString usingLicense = UsingLicense;
//	static const quint32 usingLicenseVersion = UsingLicenseVersion;
//	static const QString usingSdk = UsingSDK;
//	static const quint32 usingSdkVersion = UsingSDKVersion;
//	static const quint32 applicationVersion = ApplicationVersion;
//	static const quint32 version = applicationVersion;
//	static const quint16 applicationLanguage = ApplicationLanguage;

	static QString getVersionString()/* const*/{
		About _about;
		QString versionEString = _about.getVersionEditionInTheMap((ApplicationVersion & 0xFF000000) >> 24);
		int majorVersion = (ApplicationVersion & 0x00FF0000) >> 16;
		int minorVersion = (ApplicationVersion & 0x0000FF00) >> 8;
		int patchVersion = (ApplicationVersion & 0x000000FF);
		if(!versionEString.isEmpty()){
			return QString("%1.%2%3 %4").arg(majorVersion).arg(minorVersion).arg((patchVersion == 0)?"":QString("(%1)").arg(patchVersion)).arg(versionEString);
		}else{
			return QString("%1.%2%3").arg(majorVersion).arg(minorVersion).arg((patchVersion == 0)?"":QString("(%1)").arg(patchVersion));
		}
	}

	static void showAboutDialog(){
		QMessageBox msgBox;
		msgBox.setWindowTitle(QObject::tr("About %1 V%2").arg(ApplicationName).arg(getVersionString()));
		msgBox.setText("This is About Dialog Content.");

		msgBox.exec();
	}

	static const QString appName(){
		return QApplication::applicationDisplayName();
	}

	static void showWelcomeDialog(){
		//QMessageBox msgBox;
		//msgBox.setWindowTitle(QObject::tr("Made"));
		//msgBox.setText("ようこそ。");

		//msgBox.exec();
	}
};
#endif // If Switch

#endif // ABOUT_H
