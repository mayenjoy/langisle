#include "stdafx.h"
#include "global.h"
#include "log.h"


QString Global::encrypt(QString data)
{
    QByteArray ba = data.toUtf8();
    for (int i = 0; i < ba.size(); i++)
    {
        ba[i] = ba[i] ^ 99;
    }
    ba = ba.toBase64();
    data = QString::fromAscii(ba.data());
    return data;
}

QString Global::uncrypt(QString data)
{
    QByteArray ba;
    ba = QByteArray::fromBase64(data.toAscii());
    for (int i = 0; i < ba.size(); i++)
    {
        ba[i] = ba[i] ^ 99;
    }
    data = data.fromUtf8(ba.data());

    return data;
}

QString Global::makePathStr(const QString &fileName)
{
    QString appPath = QCoreApplication::applicationDirPath();
    DBG(qDebug() << appPath << endl << QCoreApplication::applicationFilePath());
	return QDir::convertSeparators(appPath.append(fileName));
}

QString Global::makeRelativeFilePath(const QString &fileName)
{
	QDir dirfrom;
	return dirfrom.relativeFilePath(makePathStr(fileName));
}


void Global::mkPath(const QString &strPath)
{
	if (!QDir(strPath).exists())
		QDir().mkpath(strPath);
}

void Global::sleep(int secs)
{
	QTime dieTime = QTime::currentTime().addMSecs(secs);

	while( QTime::currentTime() < dieTime )
		QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
}

void Global::setCodec(char *codeName)
{
	QTextCodec *codecs = QTextCodec::codecForName(codeName);
	if (NULL == codecs)
		codecs = QTextCodec::codecForLocale();

	QTextCodec::setCodecForLocale(codecs);
	QTextCodec::setCodecForTr(codecs);
	QTextCodec::setCodecForCStrings(codecs);
	
}

void Global::createRelExeDir(const QString &path)
{
	QString strPath = Global::makePathStr(path);
	Global::mkPath(strPath);
}

QStringList Global::getDirFileListByFilter(QString path, QString filter)
{
	QDir dir(path);
	QStringList filters;
	filters << filter;
	dir.setNameFilters(filters);
	QStringList fileList = dir.entryList();

	QStringList slRet;
	for (int i = 0; i < fileList.size(); i++)
	{
		slRet <<  path + fileList[i];
	}

	return slRet;
}

bool Global::saveToDisk(const QString &filename, QIODevice *data)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		fprintf(stderr, "Could not open %s for writing: %s\n",
			qPrintable(filename),
			qPrintable(file.errorString()));
		return false;
	}

	file.write(data->readAll());
	file.close();

	return true;
}

bool Global::saveToDisk(const QString &filename, QByteArray &data)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		fprintf(stderr, "Could not open %s for writing: %s\n",
			qPrintable(filename),
			qPrintable(file.errorString()));
		return false;
	}

	file.write(data);
	file.close();

	return true;
}
#ifdef __WINDOWS

#include "Windows.h"

//void Global::deleteDirWin(LPCWSTR lpDir)
//{

//	SHFILEOPSTRUCT ss;
//	ZeroMemory(&ss, sizeof(SHFILEOPSTRUCT));
//	ss.wFunc = FO_DELETE;
//	ss.pFrom = lpDir;
//	ss.fFlags = FOF_NOCONFIRMATION;

//	SHFileOperation(&ss);
//}
#endif
bool Global::deleteDir(const QString &dirName) 
{
	if (dirName.isEmpty())
		return true;

	QDir directory(dirName);

	if (!directory.exists()) 
	{
		return true;
	}

	QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);


	QList<QString>::iterator f = files.begin();

	bool error = false;

	for (; f != files.end(); ++f) 
	{
		QString filePath = QDir::convertSeparators(directory.path() + '/' + (*f));
		QFileInfo fi(filePath);
		if (fi.isFile() || fi.isSymLink())
		{
			QFile::setPermissions(filePath, QFile::WriteOwner);
			if (!QFile::remove(filePath)) 
			{
				DBG(qDebug() << "Global::deleteDir 1" << filePath << "faild");
				error = true;
			}
		}
		else if (fi.isDir())
		{
			if (!deleteDir(filePath));
			{
				error = true;
			}
		}
	}

	if(!directory.rmdir(QDir::convertSeparators(directory.path()))) 
	{
		DBG(qDebug() << "Global::deleteDir 3" << directory.path()  << "faild");
		error = true;
	}

	return !error;	

}




bool Global::copyDir(const QString &source, const QString &destination, const bool override) 
{
	QDir directory(source);
	bool error = false;

	if (!directory.exists()) 
	{
		return false;
	}

	QStringList dirs = directory.entryList(QDir::AllDirs | QDir::Hidden);
	QStringList files = directory.entryList(QDir::Files | QDir::Hidden);

	QList<QString>::iterator d,f;

	for (d = dirs.begin(); d != dirs.end(); ++d) 
	{
		if ((*d) == "." || (*d) == "..") 
		{
			continue;
		}

		if (!QFileInfo(directory.path() + "/" + (*d)).isDir()) 
		{
			continue;
		}

		QDir temp(destination + "/" + (*d));
		temp.mkpath(temp.path());

		if (!copyDir(directory.path() + "/" + (*d), destination + "/" + (*d), override)) {
			error = true;
		}
	}

	for (f = files.begin(); f != files.end(); ++f) 
	{
		QFile tempFile(directory.path() + "/" + (*f));


		if (QFileInfo(directory.path() + "/" + (*f)).isDir()) 
		{
			continue;
		}

		QFile destFile(destination + "/" + directory.relativeFilePath(tempFile.fileName()));

		if (destFile.exists() && override) 
		{
			destFile.remove();
		}

		if (!tempFile.copy(destination + "/" + directory.relativeFilePath(tempFile.fileName()))) 
		{
			error = true;

		}
	}


	return !error;
}

