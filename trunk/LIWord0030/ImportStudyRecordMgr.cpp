#include "stdafx.h"
#include "ImportStudyRecordMgr.h"
#include "ImportStudyRecordLI02X.h"
ImportStudyRecordMgr::ImportStudyRecordMgr(void)
{
	ccList.append(new ImportStudyRecordLI02X);
}

ImportStudyRecordMgr::~ImportStudyRecordMgr(void)
{
	foreach (ImportStudyRecordBase * bc, ccList)
	{
		delete bc;
	}
	ccList.clear();
}

QStringList ImportStudyRecordMgr::getConvertTypeName()
{
	QStringList sl;
	for (int i = 0; i < ccList.size(); i++)
		sl << ccList.at(i)->tname();
	return sl; 
}

bool ImportStudyRecordMgr::StartImport(UDataDB *udb, int typeIndex, const QString &path)
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	bool bRet = ccList.at(typeIndex)->StartImport(udb, path);
	QApplication::restoreOverrideCursor();
	return bRet;
}