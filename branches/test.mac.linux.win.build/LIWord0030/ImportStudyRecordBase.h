#pragma once
class UDataDB;
class ImportStudyRecordBase
{
public:
	ImportStudyRecordBase(void);
	virtual ~ImportStudyRecordBase(void);

	virtual bool StartImport(UDataDB *udb, const QString &path) = 0;

	QString tname() {return typeName;};
protected:
	QString typeName;
};
