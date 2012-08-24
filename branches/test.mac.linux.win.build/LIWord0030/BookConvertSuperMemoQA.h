#pragma once
#include "BookConvertBase.h"

class BookConvertSuperMemoQA :
	public BookConvertBase
{
public:
	BookConvertSuperMemoQA(void);
	virtual ~BookConvertSuperMemoQA(void);

	virtual bool Convert(BookDB *bdb, const QString &srcPath);
};
