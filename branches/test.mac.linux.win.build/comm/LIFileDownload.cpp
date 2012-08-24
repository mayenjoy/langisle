#include "stdafx.h"
#include "LIFileDownload.h"

LIFileDownload::LIFileDownload(QObject *parent)
	: QObject(parent)
{
	connect(&manager, SIGNAL(finished(QNetworkReply*)),
				SLOT(downloadFinished(QNetworkReply*)));


}

LIFileDownload::~LIFileDownload()
{

}

void LIFileDownload::download(QUrl url, QString filePath)
{
	reply = NULL;
	bFinish = false;
	DBG(qDebug() << "LIFileDownload::download" << url << filePath);
	fPath = filePath;
	QNetworkRequest request(url);
	reply = manager.get(request);	
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
		this, SIGNAL(downloadProgress(qint64,qint64)));
}

bool LIFileDownload::saveToDisk(const QString &filename, QIODevice *data)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug("Could not open %s for writing: %s\n",
			qPrintable(filename),
			qPrintable(file.errorString()));
		return false;
	}

	file.write(data->readAll());
	file.close();

	return true;
}

void LIFileDownload::downloadFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	if (reply->error()) {
		qDebug("Download of %s failed: %s\n",
			url.toEncoded().constData(),
			qPrintable(reply->errorString()));
	} else {
		if (saveToDisk(fPath, reply))
			qDebug("Download of %s succeeded (saved to %s)\n",
			url.toEncoded().constData(), qPrintable(fPath));
	}

	bFinish = true;
	reply->deleteLater();

	emit downloaded(reply->error());
}

void LIFileDownload::error(QNetworkReply::NetworkError code)
{
	DBG(qDebug() << "LIFileDownload::error" << code);
}

void LIFileDownload::cancel()
{
	if (!bFinish && reply)
	{
		disconnect(reply, SIGNAL(downloadProgress(qint64,qint64)),
			this, SIGNAL(downloadProgress(qint64,qint64)));

		reply->abort();
	}

	
}
