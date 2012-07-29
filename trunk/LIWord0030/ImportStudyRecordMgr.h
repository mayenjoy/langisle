#pragma once
#include "UDataDB.h"

class ImportStudyRecordBase;

class ImportStudyRecordMgr
{
public:
	ImportStudyRecordMgr(void);
	virtual ~ImportStudyRecordMgr(void);

	bool StartImport(UDataDB *udb, int typeIndex, const QString &path);

	QStringList getConvertTypeName();	// 获取所有转换类型
private:
	QList<ImportStudyRecordBase *> ccList;	// 存储所有的转换类型
};
