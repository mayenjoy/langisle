#pragma once
#include "ImportStudyRecordBase.h"
class UDataDB;
class ImportStudyRecordLI02X :
	public ImportStudyRecordBase
{
public:
	ImportStudyRecordLI02X(void);
	virtual ~ImportStudyRecordLI02X(void);

	virtual bool StartImport(UDataDB *udb, const QString &path);

	QStringList getAllStudyDBPath(const QString &path);

	void importLI02XBookDBAndStudyData(const QString &sdbFName, const QString &sdbPath);


protected:
	UDataDB *m_udb;
	QString m_path;
	QString m_uName;
	int m_cid;
};
