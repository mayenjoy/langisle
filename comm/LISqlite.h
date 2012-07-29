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

	QVariant rexec(const QString &sql);	// 执行sql语句,返回第一行第一列的值
	uint  lastInsertID();	// 取最后插入数据的ID
	bool exec(const QString &sql); // 执行不需要返回结果sql语句

	bool isTableExist(const QString &tableName);	// 给定表名是否存在
	QSqlDatabase *getdb() {return db;};
protected:
	QSqlDatabase *db;
	QString m_connName;
};
