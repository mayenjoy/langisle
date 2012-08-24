#pragma once
#include "../comm/LISqlite.h"

class LIWord02xBookDB :
	public LISqlite
{
public:
	LIWord02xBookDB(const QString &dbpath, const QString &connName);
	virtual ~LIWord02xBookDB(void);

	void getAllWord(QList< QMap<QString, QString> > &wList);
};
