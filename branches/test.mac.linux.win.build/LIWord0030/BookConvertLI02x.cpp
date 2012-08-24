#include "stdafx.h"
#include "BookConvertLI02x.h"
#include "LIWord02xBookDB.h"
BookConvertLI02x::BookConvertLI02x(void)
{
	typeName = "LIWord0.2.x´Ê¿â";
}

BookConvertLI02x::~BookConvertLI02x(void)
{
}

bool BookConvertLI02x::Convert(BookDB *bdb, const QString &srcPath)
{
	LIWord02xBookDB li02x(srcPath, srcPath);

	QList< QMap<QString, QString> > wList;
	li02x.getAllWord(wList);

	bdb->addEditWord(wList);

	return true;
}
