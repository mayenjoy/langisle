#pragma once
class BookConvertBase;
class BookDB;
class BookConvertMgr
{
public:
	BookConvertMgr(void);
	virtual ~BookConvertMgr(void);

	QStringList getConvertTypeName();	// ��ȡ����ת������
	bool Convert(BookDB *bdb, int typeIndex, const QString &srcPath);
private:
	QList<BookConvertBase *> ccList;	// �洢���е�ת������
};
