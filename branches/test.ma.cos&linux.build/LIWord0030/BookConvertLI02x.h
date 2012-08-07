#pragma once
#include "BookConvertBase.h"

class BookConvertLI02x :
	public BookConvertBase
{
public:
	BookConvertLI02x(void);
	virtual ~BookConvertLI02x(void);

	virtual bool Convert(BookDB *bdb, const QString &srcPath);
};
