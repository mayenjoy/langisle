#pragma once
#include "BookConvertBase.h"

class BookConvertCiba :
	public BookConvertBase
{
public:
	BookConvertCiba(void);
	virtual ~BookConvertCiba(void);

	virtual bool Convert(BookDB *bdb, const QString &srcPath);
};
