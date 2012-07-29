#pragma once
#include "UDataDB.h"

class ImportStudyRecordBase;

class ImportStudyRecordMgr
{
public:
	ImportStudyRecordMgr(void);
	virtual ~ImportStudyRecordMgr(void);

	bool StartImport(UDataDB *udb, int typeIndex, const QString &path);

	QStringList getConvertTypeName();	// ��ȡ����ת������
private:
	QList<ImportStudyRecordBase *> ccList;	// �洢���е�ת������
};
