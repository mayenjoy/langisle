#include "stdafx.h"
#include "BookConvertMgr.h"
#include "BookConvertCiba.h"
#include "BookConvertYoudao.h"
#include "BookConvertLI02x.h"
#include "BookConvertSuperMemoQA.h"
BookConvertMgr::BookConvertMgr(void)
{
	ccList.append(new BookConvertYoudao);
	ccList.append(new BookConvertCiba);	
	ccList.append(new BookConvertLI02x);
	ccList.append(new BookConvertSuperMemoQA);	
	
}

BookConvertMgr::~BookConvertMgr(void)
{
	foreach (BookConvertBase * bc, ccList)
	{
		delete bc;
	}
	ccList.clear();
}

QStringList BookConvertMgr::getConvertTypeName()
{
	QStringList sl;
	for (int i = 0; i < ccList.size(); i++)
		sl << ccList.at(i)->tname();
	return sl; 
}

bool BookConvertMgr::Convert(BookDB *bdb, int typeIndex, const QString &srcPath)
{
	DBG(qDebug() << "BookConvertMgr::Convert สนำร" << ccList.at(typeIndex)->tname(););
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	bool bRet = ccList.at(typeIndex)->Convert(bdb, srcPath);
	QApplication::restoreOverrideCursor();
	return bRet;
}
