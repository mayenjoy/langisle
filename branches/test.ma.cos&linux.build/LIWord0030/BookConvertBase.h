#pragma once
#include "BookDB.h"
class BookConvertBase
{
public:
	BookConvertBase(void);
	virtual ~BookConvertBase(void);

	QString tname() {return typeName;};
	virtual bool Convert(BookDB *bdb, const QString &srcPath) = 0;
protected:
	QString typeName;
};
