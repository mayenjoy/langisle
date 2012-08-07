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


	void checkNewVer();	// ����°汾
	bool applyNewVer();	// Ӧ���°汾

    QString updateNote() {return updateNotes;} //���ظ�����־
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
