#include "stdafx.h"
#include "SpeakYodao.h"
#include "GlobalVal.h"
SpeakYodao::SpeakYodao(void) : SpeakOnlineBase("SpeakYodao")
{
}

SpeakYodao::~SpeakYodao(void)
{
}

QUrl SpeakYodao::makeUrl(const QString &text)
{
	//return QString(YOUDAO_URL_MP3 "%1").arg(text);
	QUrl url(YOUDAO_URL_MP3);
	url.addQueryItem("audio", text);
	return url;
}

void SpeakYodao::reset()
{
	DBG(qDebug() << "SpeakYodao::reset");
}