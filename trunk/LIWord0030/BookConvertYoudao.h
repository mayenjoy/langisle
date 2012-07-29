#pragma once
#include "BookConvertBase.h"

class BookConvertYoudao :
	public BookConvertBase
{
public:
	BookConvertYoudao(void);
	virtual ~BookConvertYoudao(void);

	virtual bool Convert(BookDB *bdb, const QString &srcPath);
};

