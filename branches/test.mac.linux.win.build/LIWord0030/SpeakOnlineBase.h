#ifndef SPEAKONLINEBASE_H
#define SPEAKONLINEBASE_H

#include "SpeakBase.h"
#include <QtNetwork>
class SpeakOnlineBase : public SpeakBase
{
	Q_OBJECT

public:
	SpeakOnlineBase(const QString &name);
	~SpeakOnlineBase();

	virtual bool speak(const QString &text);
	virtual void stop();
	virtual QUrl makeUrl(const QString &text) = 0;
public slots:
	void downloadFinished(QNetworkReply *reply);
protected:
	QList<QString> wordDownList;
	QMutex mutex;
	QNetworkAccessManager manager;
	QList<QNetworkReply *> currentDownloads;
	QString curPath;
};

#endif // SPEAKONLINEBASE_H
