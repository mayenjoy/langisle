#pragma once
class BookConvertBase;
class BookDB;
class BookConvertMgr
{
public:
	BookConvertMgr(void);
	virtual ~BookConvertMgr(void);

	QStringList getConvertTypeName();	// 获取所有转换类型
	bool Convert(BookDB *bdb, int typeIndex, const QString &srcPath);
private:
	QList<BookConvertBase *> ccList;	// 存储所有的转换类型
};
