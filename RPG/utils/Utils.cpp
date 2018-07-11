#include "Utils.h"

void Utils::msleep(int msec){
	QEventLoop eventLoop;
	QTimer::singleShot(msec, &eventLoop, SLOT(quit()));
	eventLoop.exec();
}

QByteArray Utils::readFile(const QString &fileName){
	QFile f(fileName);
	if(!f.open(QIODevice::ReadOnly)){
		qDebug() << CodePath() << "Cannot open" << fileName << "to read information.";
		return QByteArray();
	}
	QByteArray data = f.readAll();
	f.close();
	if(data.isEmpty()){
		qDebug() << CodePath() << "File" << fileName << "content empty";
		return data;
	}
	return data;
}

QString Utils::detectedJsonValue(const QJsonValue &value){
	QString type;
	if(value.isNull()){
		type = "Null";
	}else if(value.isArray()){
		type = "Array";
	}else if(value.isObject()){
		type = "Object";
	}else if(value.isBool()){
		type = "Bool";
	}else if(value.isDouble()){
		type = "Double";
	}else if(value.isString()){
		type = "String";
	}else if(value.isUndefined()){
		type = "Undefined";
	}else{
		type = "Unknown";
	}
	return type;
}

QString Utils::_keyModifierToString(Qt::KeyboardModifiers mod){
	QStringList modList;
	if(mod & Qt::ControlModifier){
#ifdef Q_OS_MACOS
		modList.append("Command");
#else
		modList.append("Ctrl");
#endif
	}
	if(mod & Qt::MetaModifier){
#ifdef Q_OS_MACOS
		modList.append("Control");
#else
		modList.append("Meta");
#endif
	}
	if(mod & Qt::AltModifier){
		modList.append("Alt");
	}
	if(mod & Qt::ShiftModifier){
		modList.append("Shift");
	}
	return modList.join(" + ");
}

QString Utils::_keyToString(Qt::Key key){
	switch(key){
		case Qt::Key_Escape: return "Esc";
		case Qt::Key_Tab: return "Tab";
		case Qt::Key_Backtab: return "Backtab";
		case Qt::Key_Backspace: return "Backspace";
		case Qt::Key_Return: return "Enter";
		case Qt::Key_Enter: return "Enter(KP)";
		case Qt::Key_Insert: return "Insert";
		case Qt::Key_Delete: return "Delete";
		case Qt::Key_Pause: return "Pause";
		case Qt::Key_Print: return "Print";
		case Qt::Key_SysReq: return "SysReq";
		case Qt::Key_Clear: return "Clear";
		case Qt::Key_Home: return "Home";
		case Qt::Key_End: return "End";
		case Qt::Key_Left: return "←";
		case Qt::Key_Right: return "→";
		case Qt::Key_Up: return "↑";
		case Qt::Key_Down: return "↓";
		case Qt::Key_PageUp: return "Page Up";
		case Qt::Key_PageDown: return "Page Down";
		case Qt::Key_Shift: return "Shift";
		case Qt::Key_Control: return "Control";
		case Qt::Key_Meta: return "Meta";
		case Qt::Key_Alt: return "Alt";
		case Qt::Key_AltGr: return "AltGr";
		case Qt::Key_CapsLock: return "Caps Lock";
		case Qt::Key_NumLock: return "Num Lock";
		case Qt::Key_ScrollLock: return "Scroll Lock";
		case Qt::Key_F1: return "F1";
		case Qt::Key_F2: return "F2";
		case Qt::Key_F3: return "F3";
		case Qt::Key_F4: return "F4";
		case Qt::Key_F5: return "F5";
		case Qt::Key_F6: return "F6";
		case Qt::Key_F7: return "F7";
		case Qt::Key_F8: return "F8";
		case Qt::Key_F9: return "F9";
		case Qt::Key_F10: return "F10";
		case Qt::Key_F11: return "F11";
		case Qt::Key_F12: return "F12";
		case Qt::Key_F13: return "F13";
		case Qt::Key_F14: return "F14";
		case Qt::Key_F15: return "F15";
		case Qt::Key_F16: return "F16";
		case Qt::Key_F17: return "F17";
		case Qt::Key_F18: return "F18";
		case Qt::Key_F19: return "F19";
		case Qt::Key_F20: return "F20";
		case Qt::Key_F21: return "F21";
		case Qt::Key_F22: return "F22";
		case Qt::Key_F23: return "F23";
		case Qt::Key_F24: return "F24";
		case Qt::Key_F25: return "F25";
		case Qt::Key_F26: return "F26";
		case Qt::Key_F27: return "F27";
		case Qt::Key_F28: return "F28";
		case Qt::Key_F29: return "F29";
		case Qt::Key_F30: return "F30";
		case Qt::Key_F31: return "F31";
		case Qt::Key_F32: return "F32";
		case Qt::Key_F33: return "F33";
		case Qt::Key_F34: return "F34";
		case Qt::Key_F35: return "F35";
		case Qt::Key_Super_L: return "Super L";
		case Qt::Key_Super_R: return "Super R";
		case Qt::Key_Menu: return "Menu";
		case Qt::Key_Hyper_L: return "Hyper L";
		case Qt::Key_Hyper_R: return "Hyper R";
		case Qt::Key_Help: return "Help";
		case Qt::Key_Direction_L: return "Key Direction L";
		case Qt::Key_Direction_R: return "Key Direction R";
		case Qt::Key_Space: return "<space>";
		case Qt::Key_Exclam ... Qt::Key_AsciiTilde: return QString(QChar(key));
		case Qt::Key_nobreakspace: return "<nbsp>";
		case Qt::Key_exclamdown: return "¡";
		case Qt::Key_cent: return "￠";
		case Qt::Key_sterling: return "£";
		case Qt::Key_currency: return "¤";
		case Qt::Key_yen: return "¥";
			/*_*/
		case Qt::Key_Back: return "Back";
		case Qt::Key_Forward: return "Forward";
		case Qt::Key_Stop: return "Stop";
		case Qt::Key_Refresh: return "Refresh";
		case Qt::Key_VolumeDown: return "Volume Down";
		case Qt::Key_VolumeMute: return "Volume Mute";
		case Qt::Key_VolumeUp: return "Volume Up";
		case Qt::Key_BassBoost: return "Bass Boost";
		case Qt::Key_BassUp: return "Bass Up";
		case Qt::Key_BassDown: return "Bass Down";
		case Qt::Key_TrebleUp: return "Treble Up";
		case Qt::Key_TrebleDown: return "Treble Down";
		case Qt::Key_MediaPlay: return "Media Play";
		case Qt::Key_MediaStop: return "Media Stop";
		case Qt::Key_MediaPrevious: return "Media Previous";
		case Qt::Key_MediaNext: return "Media Next";
		case Qt::Key_MediaRecord: return "Media Record";
		case Qt::Key_MediaPause: return "Media Pause";
		case Qt::Key_MediaTogglePlayPause: return "Media Toggle Play Pause";
		case Qt::Key_HomePage: return "Home Page";
		case Qt::Key_Favorites: return "Favorites";
		case Qt::Key_Search: return "Search";
		case Qt::Key_Standby: return "Stand By";
		case Qt::Key_OpenUrl: return "Open Url";
		case Qt::Key_LaunchMail: return "Launch Mail";
		case Qt::Key_LaunchMedia: return "Launch Media";
		default: return EnumKeyToString(key);
	}
}
