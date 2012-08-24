#include "stdafx.h"
#include "SpeakGoogle.h"

SpeakGoogle::SpeakGoogle(QObject *parent)
	: SpeakOnlineBase("SpeakGoogle")
{

}

SpeakGoogle::~SpeakGoogle()
{

}

QUrl SpeakGoogle::makeUrl(const QString &text)
{
	DBG(qDebug() << "SpeakGoogle::makeUrl" << text);
	// QString url =  QString(GOOGLE_URL_MP3).arg(text);
	// DBG(qDebug() << "SpeakGoogle::makeUrl" << QUrl::fromUserInput(text).toString() << url);
	//DBG(qDebug() << "SpeakGoogle::makeUrl" << QUrl::fromEncoded(QUrl::toPercentEncoding(text, "", "&"), QUrl::StrictMode));
	QUrl url(GOOGLE_URL_MP3);
	url.addQueryItem("q", text);
	url.addQueryItem("tl", "en");
	url.addQueryItem("prev", "input");
	return url;
}

void SpeakGoogle::reset()
{
	DBG(qDebug() << "SpeakGoogle::reset");
}