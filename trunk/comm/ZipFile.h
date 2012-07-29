#ifndef ZIPFILE_H
#define ZIPFILE_H

#include "../comm/quazip/quazip/quazip.h"
#include "../comm/quazip/quazip/quazipfile.h"

class ZipFile : public QObject
{
	Q_OBJECT
public:
	ZipFile();
	~ZipFile();

	bool extract(const QString & filePath, const QString & extDirPath, const QString & singleFileName = QString(""));
	int getfileCount() {return fileCount;}
signals:
	void currentProgress ( qint64 current, qint64 total );
	void finished(int ret);
public slots:
	void cancel();
private:
	QuaZip zip;
	volatile bool stop;
	int fileCount;
};

#endif // ZIPFILE_H
