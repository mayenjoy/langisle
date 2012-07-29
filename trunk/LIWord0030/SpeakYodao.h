#pragma once
#include "SpeakOnlineBase.h"

#define YOUDAO_URL_MP3 "http://dict.youdao.com/dictvoice"
//translate.google.cn/translate_tts?q=hello&tl=en&prev=input
class SpeakYodao :
	public SpeakOnlineBase
{
	Q_OBJECT
public:
	SpeakYodao(void);
	virtual ~SpeakYodao(void);

	virtual QUrl makeUrl(const QString &text);
	virtual void reset();
};
