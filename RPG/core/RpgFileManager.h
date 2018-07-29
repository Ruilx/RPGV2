#ifndef RPGFILEMANAGER_H
#define RPGFILEMANAGER_H

#include <QtCore>

#define toChars(x) #x

class RpgFileManager
{
	static RpgFileManager *_instance;
public:
	enum FileType{
		Unknown = 0,
		NormalFile,
		FontFile,
		MapFile,
		ImageFile,
		MusicFile,
		LyricFile,
		SeFile,
		ScriptFile,
		AvatarFile,
	};

	static RpgFileManager *instance(){
		if(_instance == nullptr){
			_instance = new RpgFileManager();
		}
		return _instance;
	}
private:

	QHash<FileType, QHash<QString, QUrl>* > files;

	QHash<FileType, QString> fileTypeName;
public:
	RpgFileManager(){
		for(QHash<QString, QUrl> *file: this->files){
			if(file == nullptr){
				continue;
			}
			file->clear();
			delete file;
			file = nullptr;
		}
		this->files.clear();

		this->files.insert(Unknown, new QHash<QString, QUrl>());
		this->files.insert(NormalFile, new QHash<QString, QUrl>());
		this->files.insert(FontFile, new QHash<QString, QUrl>());
		this->files.insert(MapFile, new QHash<QString, QUrl>());
		this->files.insert(ImageFile, new QHash<QString, QUrl>());
		this->files.insert(MusicFile, new QHash<QString, QUrl>());
		this->files.insert(LyricFile, new QHash<QString, QUrl>());
		this->files.insert(SeFile, new QHash<QString, QUrl>());
		this->files.insert(ScriptFile, new QHash<QString, QUrl>());
		this->files.insert(AvatarFile, new QHash<QString, QUrl>());

		this->fileTypeName.insert(Unknown, "Unknown");
		this->fileTypeName.insert(NormalFile, "NormalFile");
		this->fileTypeName.insert(FontFile, "FontFile");
		this->fileTypeName.insert(MapFile, "MapFile");
		this->fileTypeName.insert(ImageFile, "ImageFile");
		this->fileTypeName.insert(MusicFile, "MusicFile");
		this->fileTypeName.insert(LyricFile, "LyricFile");
		this->fileTypeName.insert(SeFile, "SeFile");
		this->fileTypeName.insert(ScriptFile, "ScriptFile");
		this->fileTypeName.insert(AvatarFile, "AvatarFile");
	}

	void addFile(FileType type, const QString &name, const QUrl &url){
		if(!this->files.contains(type)){
			return;
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			return;
		}
		file->insert(name, url);
	}

	void removeFile(FileType type, const QString &name){
		if(!this->files.contains(type)){
			return;
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			return;
		}
		file->remove(name);
	}

	QUrl getFile(FileType type, const QString &name) const{
		if(!this->files.contains(type)){
			return QUrl();
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			return QUrl();
		}
		return file->value(name);
	}

	QString getFileString(FileType type, const QString &name){
		QUrl url = this->getFile(type, name);
		if(!url.isValid()){
			return QString();
		}
		return url.url(QUrl::PreferLocalFile);
	}

	void _dumpFiles(){
		for(QHash<FileType, QHash<QString, QUrl>* >::const_iterator i = this->files.constBegin(); i != this->files.constEnd(); i++){
			qDebug() << "========================" << this->fileTypeName.value(i.key(), "UNKNOWN") << "========================";
			QHash<QString, QUrl> *file = i.value();
			if(file == nullptr){
				qDebug() << "NULL POINTER OCCURRED!";
				continue;
			}
			for(QHash<QString, QUrl>::const_iterator ii = file->constBegin(); ii != file->constEnd(); ii++){
				qDebug() << ii.key() << " => " << ii.value();
			}
		}

	}
};

#endif // RPGFILEMANAGER_H
