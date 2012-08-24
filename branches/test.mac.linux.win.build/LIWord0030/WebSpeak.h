#ifndef WEBSPEAK_H
#define WEBSPEAK_H

#include "SpeakOnlineBase.h"

class WebSpeak : public SpeakOnlineBase
{
	Q_OBJECT

public:
	WebSpeak(QObject *parent = 0);
	~WebSpeak();

	QUrl makeUrl(const QString &text);
	bool speakUrl(const QUrl &url);
private:
	QUrl m_url;
};

#endif // WEBSPEAK_H
