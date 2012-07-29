#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class LISqlite
{
public:
	LISqlite(const QString &dbpath, const QString &connName);
	virtual ~LISqlite(void);

	QVariant rexec(const QString &sql);	// ִ��sql���,���ص�һ�е�һ�е�ֵ
	uint  lastInsertID();	// ȡ���������ݵ�ID
	bool exec(const QString &sql); // ִ�в���Ҫ���ؽ��sql���

	bool isTableExist(const QString &tableName);	// ���������Ƿ����
	QSqlDatabase *getdb() {return db;};
protected:
	QSqlDatabase *db;
	QString m_connName;
};
