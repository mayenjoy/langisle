#pragma once
#include "../comm/LISqlite.h"
#include "config.h"
class LIWord02xStudyDB :
	public LISqlite
{
public:
	LIWord02xStudyDB(const QString &dbpath, const QString &connName);
	virtual ~LIWord02xStudyDB(void);

	QList<WordInfo> getWordInfoList(const QString &sdbPath);
};
