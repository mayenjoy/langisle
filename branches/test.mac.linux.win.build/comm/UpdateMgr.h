#ifndef UPDATEMGR_H
#define UPDATEMGR_H
#include <QObject>

#include "LIFileDownload.h"




class UpdateMgr : public QObject
{
	Q_OBJECT

public:
	UpdateMgr(QObject *parent = NULL);
	~UpdateMgr();


	void checkNewVer();	// 检测新版本
	bool applyNewVer();	// 应用新版本

    QString updateNote() {return updateNotes;} //返回更新日志
	QString getResultInfo() {return resultInfo;};
signals:
	void checkResult(bool bHaveNewVer);
public slots:
	void downloaded(int error);
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
	void parse(QString verFile);	
private:
	LIFileDownload downer;
	QString vernumber;
	QString downurl;
	QString updateNotes;
	QString resultInfo;
};

#endif // UPDATEMGR_H
