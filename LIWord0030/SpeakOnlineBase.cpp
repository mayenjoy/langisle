#include "stdafx.h"
#include "SpeakOnlineBase.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "SoundFilePlayMgr.h"
#define CACHE_PATH "/cache"
SpeakOnlineBase::SpeakOnlineBase(const QString &name)
	: SpeakBase(name)
{
	Global::createRelExeDir(CACHE_PATH);

	connect(&manager, SIGNAL(finished(QNetworkReply*)),
		SLOT(downloadFinished(QNetworkReply*)));

	connect(GlobalVal::s_psfpm, SIGNAL(finished()), this, SIGNAL(finished()));
}

SpeakOnlineBase::~SpeakOnlineBase()
{

}

bool SpeakOnlineBase::speak(const QString &text)
{
	//qDebug() << "SpeakOnlineBase::speak" << text;
	DBG(qDebug() << "SpeakOnlineBase::speak 1" << text);
	if (text.isEmpty())
	{
		DBG(qDebug() << "SpeakOnlineBase::Speak 播放内容为空" << text);
		return false;
	}
	GlobalVal::s_psfpm->stop();
	QString hash = QString(QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5).toHex());
    QString strPath = Global::makePathStr(QString(CACHE_PATH "/" + m_name + "/%1/%2/%3.mp3").arg(text.at(0).toLower()).arg(hash.mid(0, 2).toLower()).arg(hash));
	DBG(qDebug() << "SpeakOnlineBase::speak : 2" << strPath);
	mutex.lock();
	curPath = strPath;
	mutex.unlock();

	DBG(qDebug() << "SpeakOnlineBase::speak 3" << strPath);
	if (QFile::exists(strPath))
	{
		// 直硬
		GlobalVal::s_psfpm->stop();
		GlobalVal::s_psfpm->play(strPath);
	}
	else
	{
		
		mutex.lock();
		if (wordDownList.contains(strPath))
		{
			qDebug() << "重入" << text;
			mutex.unlock();
			return false;
		}
		wordDownList.append(strPath);
		mutex.unlock();
		
		//////////////////////////////////////////////////////////////////////////

		Global::createRelExeDir(QString(CACHE_PATH "/" + m_name + "/%1").arg(text.at(0).toLower()));
		Global::createRelExeDir(QString(CACHE_PATH  "/" + m_name + "/%1/%2").arg(text.at(0).toLower()).arg(hash.mid(0, 2).toLower()));


		QNetworkRequest request(makeUrl(text));
		request.setAttribute(QNetworkRequest::User, strPath);
		QNetworkReply *reply = manager.get(request);	

		currentDownloads.append(reply);
	}
	DBG(qDebug() << "SpeakOnlineBase::speak 4");
	return true;

}

void SpeakOnlineBase::stop()
{
	GlobalVal::s_psfpm->stop();
}

void SpeakOnlineBase::downloadFinished(QNetworkReply *reply)
{
	QString filePath = reply->request().attribute(QNetworkRequest::User).toString();
	QUrl url = reply->url();
	if (reply->error()) {
		qDebug("Download of %s failed: %s\n",
			url.toEncoded().constData(),
			qPrintable(reply->errorString()));
		
	} else {
		char buf[256] = "";
		reply->peek(buf, 255); // 不要用read

		DBG(qDebug() << "SpeakOnlineBase::downloadFinished" << buf);
		if (!QString::fromUtf8(buf, 256).contains("<html>", Qt::CaseInsensitive))
		{
			//DBG(qDebug() << "SpeakOnlineBase::downloadFinished ok" << filePath);
			//if (reply->d)

			Global::saveToDisk(filePath, reply);

			mutex.lock();
			if (curPath == filePath)
			{
				GlobalVal::s_psfpm->stop();
				GlobalVal::s_psfpm->play(filePath);
			}
			mutex.unlock();
		}
		else
		{
			DBG(qDebug() << "流量限制..");
		}





	}
	//
	mutex.lock();
	if (wordDownList.contains(filePath))
	{
		wordDownList.removeAll(filePath);
	}
	mutex.unlock();

	currentDownloads.removeAll(reply);
	reply->deleteLater();

	DBG(qDebug() << "SpeakOnlineBase::downloadFinished 2");
}
