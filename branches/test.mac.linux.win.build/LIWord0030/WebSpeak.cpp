#include "StdAfx.h"
#include "WebSpeak.h"

WebSpeak::WebSpeak(QObject *parent)
	: SpeakOnlineBase("WebSpeak")
{

}

WebSpeak::~WebSpeak()
{

}

bool WebSpeak::speakUrl(const QUrl &url)
{
	m_url = url;

	return SpeakOnlineBase::speak(QCryptographicHash::hash(url.toString().toUtf8(), QCryptographicHash::Md5).toHex());
}

QUrl WebSpeak::makeUrl(const QString &text)
{
	return m_url;
}


