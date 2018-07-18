#ifndef RPGFILEMANAGER_H
#define RPGFILEMANAGER_H

#include <QtCore>

class RpgFileManager
{
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
	};

private:

	QHash<FileType, (*QHash<QString, QUrl>) > files;
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

	QUrl getFile(FileType type, const QString &name){
		if(!this->files.contains(type)){
			return;
		}
		QHash<QString, QUrl> *file = this->files.value(type);
		if(file == nullptr){
			return;
		}
		return file->value(name);
	}
};

#endif // RPGFILEMANAGER_H
