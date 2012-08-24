#pragma once
#include "../comm/LISqlite.h"

class LIWord02xCategoryDB :
	public LISqlite
{
public:
	LIWord02xCategoryDB(const QString &dbpath, const QString &connName);
	virtual ~LIWord02xCategoryDB(void);

	QString getBookName(int mid);
};
