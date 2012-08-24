#include "stdafx.h"
#include "ZipFile.h"

ZipFile::ZipFile()

{
	stop = false;
}

ZipFile::~ZipFile()
{

}

bool	ZipFile::extract(const QString & filePath, const QString & extDirPath, const QString & singleFileName) 
{

	qDebug() << "ZipFile::extract 1";
	QuaZip zip(filePath);

	if (!zip.open(QuaZip::mdUnzip)) {
		DBG(qDebug("testRead(): zip.open(): %d", zip.getZipError()));
		emit finished(-1);
		return false;
	}

	qDebug() << "ZipFile::extract 2";
	//zip.setFileNameCodec("IBM866");
    //DBG(qDebug() << "FileNameCodec is" << zip.getFileNameCodec()->name());
    //DBG(qDebug("%d entries\n", zip.getEntriesCount()));
    //DBG(qDebug("Global comment: %s\n", zip.getComment().toLocal8Bit().constData()));

	qDebug() << "ZipFile::extract 2.5";
	fileCount = zip.getEntriesCount();
	qDebug() << "ZipFile::extract 2.6";

	QuaZipFileInfo info;

	QuaZipFile file(&zip);

	qDebug() << "ZipFile::extract 3";
	QFile out;
	QString name;
	char c;
	int count = 0;
	for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {

		if (stop)
		{
			emit finished(-2);
			DBG(qDebug() << "ÓÃ»§ÖÐÖ¹");
			return false;
		}
		if (!zip.getCurrentFileInfo(&info)) {
			DBG(qDebug("testRead(): getCurrentFileInfo(): %d\n", zip.getZipError()));
			emit finished(-1);
			return false;
		}

		if (!singleFileName.isEmpty())
			if (!info.name.contains(singleFileName))
				continue;

		if (!file.open(QIODevice::ReadOnly)) {
			DBG(qDebug("testRead(): file.open(): %d", file.getZipError()));
			emit finished(-1);
			return false;
		}

		name = QDir::convertSeparators(QString("%1/%2").arg(extDirPath).arg(file.getActualFileName()));

		if (file.getZipError() != UNZ_OK) {
			DBG(qDebug("testRead(): file.getFileName(): %d", file.getZipError()));
			emit finished(-1);
			return false;
		}

 		QDir dir;
 		dir.mkpath(QFileInfo(name).path());
		//out.setFileName("out/" + name);
		out.setFileName(name);

		// this will fail if "name" contains subdirectories, but we don't mind that
		out.open(QIODevice::WriteOnly);
		// Slow like hell (on GNU/Linux at least), but it is not my fault.
		// Not ZIP/UNZIP package's fault either.
		// The slowest thing here is out.putChar(c).
		while (file.getChar(&c)) out.putChar(c);

		out.close();

		if (file.getZipError() != UNZ_OK) {
			DBG(qDebug("testRead(): file.getFileName(): %d", file.getZipError()));
			emit finished(-1);
			return false;
		}

		if (!file.atEnd()) {
			DBG(qDebug() << "testRead(): read all but not EOF");
			emit finished(-1);
			return false;
		}

		file.close();

		if (file.getZipError() != UNZ_OK) {
			DBG(qDebug("testRead(): file.close(): %d", file.getZipError()));
			emit finished(-1);
			return false;
		}
		count++;
		emit currentProgress(count, fileCount);
	}

	zip.close();
	if (zip.getZipError() != UNZ_OK) {
		DBG(qDebug("testRead(): zip.close(): %d", zip.getZipError()));
		return false;
	}
	emit finished(1);
	return true;
}

static bool archive(const QString & filePath, const QDir & dir, const QString & comment = QString("")) {

	QuaZip zip(filePath);
	zip.setFileNameCodec("IBM866");

	if (!zip.open(QuaZip::mdCreate)) {
		//myMessageOutput(true, QtDebugMsg, QString("testCreate(): zip.open(): %1").arg(zip.getZipError()));
		return false;
	}

	if (!dir.exists()) {
		//myMessageOutput(true, QtDebugMsg, QString("dir.exists(%1)=FALSE").arg(dir.absolutePath()));
		return false;
	}

	QFile inFile;


	QStringList sl;
	//recurseAddDir(dir, sl);


	QFileInfoList files;
	foreach (QString fn, sl) files << QFileInfo(fn);

	QuaZipFile outFile(&zip);

	char c;
	foreach(QFileInfo fileInfo, files) {

		if (!fileInfo.isFile())
			continue;

		QString fileNameWithRelativePath = fileInfo.filePath().remove(0, dir.absolutePath().length() + 1);

		inFile.setFileName(fileInfo.filePath());

		if (!inFile.open(QIODevice::ReadOnly)) {
			//myMessageOutput(true, QtDebugMsg, QString("testCreate(): inFile.open(): %1").arg(inFile.errorString().toLocal8Bit().constData()));
			return false;
		}

		if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath()))) {
			//myMessageOutput(true, QtDebugMsg, QString("testCreate(): outFile.open(): %1").arg(outFile.getZipError()));
			return false;
		}

		while (inFile.getChar(&c) && outFile.putChar(c));

		if (outFile.getZipError() != UNZ_OK) {
			//myMessageOutput(true, QtDebugMsg, QString("testCreate(): outFile.putChar(): %1").arg(outFile.getZipError()));
			return false;
		}

		outFile.close();

		if (outFile.getZipError() != UNZ_OK) {
			//myMessageOutput(true, QtDebugMsg, QString("testCreate(): outFile.close(): %1").arg(outFile.getZipError()));
			return false;
		}

		inFile.close();
	}


	if (!comment.isEmpty())
		zip.setComment(comment);

	zip.close();

	if (zip.getZipError() != 0) {
		//myMessageOutput(true, QtDebugMsg, QString("testCreate(): zip.close(): %1").arg(zip.getZipError()));
		return false;
	}

	return true;
}

void ZipFile::cancel()
{
	stop = true;
}
