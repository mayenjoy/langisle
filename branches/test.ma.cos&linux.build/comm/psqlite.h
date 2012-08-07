#pragma once

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
class psqlite
{
public:
	psqlite(void);
	virtual ~psqlite(void);
    bool open(QString zdbpath, QString connName = "");
	bool close();
	bool exec(QString sql);
	QVariant rexec(QString sql);	// 只有一个返回结果的语句
	QVariant rexec(QString itemName, QString sql);
	int lastInsertID(); // 最后插入数据的id

	void printData(QString sql);
	QSqlDatabase *db;
	QString m_dbpath;
	QString m_connName;
};
