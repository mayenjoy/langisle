#ifndef LIFILEDOWNLOAD_H
#define LIFILEDOWNLOAD_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class LIFileDownload : public QObject
{
	Q_OBJECT

public:
	LIFileDownload(QObject *parent = NULL);
	~LIFileDownload();
	QNetworkAccessManager manager;
	QString getErrorInfo()
	{
		return reply->errorString();
	}
signals:
	void downloaded(int error);
	void	downloadProgress ( qint64 bytesReceived, qint64 bytesTotal );
public slots:
	void download(QUrl url, QString filePath);
	void error(QNetworkReply::NetworkError code);
	void cancel(); // È¡Ïû
private slots:
	void downloadFinished(QNetworkReply *reply);
	
private:
	bool saveToDisk(const QString &filename, QIODevice *data);
	
	QString fPath;
	QNetworkReply *reply;
	bool bFinish;
};

#endif // LIFILEDOWNLOAD_H
