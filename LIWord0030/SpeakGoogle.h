#ifndef SPEAKGOOGLE_H
#define SPEAKGOOGLE_H

#include "SpeakOnlineBase.h"

//#define GOOGLE_URL_MP3 "http://translate.google.cn/translate_tts?q=%1&tl=en&prev=input"
#define GOOGLE_URL_MP3 "http://translate.google.cn/translate_tts"
class SpeakGoogle : public SpeakOnlineBase
{
	Q_OBJECT

public:
	SpeakGoogle(QObject *parent = 0);
	~SpeakGoogle();

	QUrl makeUrl(const QString &text);
	virtual void reset();
};

#endif // SPEAKGOOGLE_H
