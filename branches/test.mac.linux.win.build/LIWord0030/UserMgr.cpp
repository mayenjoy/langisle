#include "stdafx.h"
#include "UserMgr.h"


UserMgr::UserMgr(const QString &dbpath, const QString &connName) : LISqlite(dbpath, connName)
{
	if (!isTableExist("TUserInfo"))	// 新建的数据库,创建表
	{
		createTable();
		QMap<QString, QVariant> ui;
		ui["userName"] = "default";
		ui["password"] = "";
		addUser(ui);

	}
}

UserMgr::~UserMgr(void)
{

}

bool UserMgr::createTable()	// 创建表
{
	QSqlQuery query(*db);

	query.exec("CREATE TABLE [TUserInfo] (\
			   [id] INTEGER NOT NULL ON CONFLICT FAIL PRIMARY KEY AUTOINCREMENT, \
			   [userName] CHAR(32) NOT NULL ON CONFLICT FAIL, \
			   [password] CHAR(32) NOT NULL ON CONFLICT FAIL, \
			   [savePsw] BOOL NOT NULL ON CONFLICT FAIL DEFAULT 0, \
			   [createTime] DATETIME NOT NULL ON CONFLICT FAIL, \
			   [lastLoginTime] DATETIME NOT NULL ON CONFLICT FAIL,\
			   [LoginCount] INTEGER NOT NULL ON CONFLICT FAIL);");

	db->commit();

	return true;
}

bool UserMgr::addUser(QMap<QString, QVariant> &ui)
{
	QString userName = ui["userName"].toString();
	QString psw = ui["password"].toString();

	if (!exists(userName))
	{
		QString strCurrentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
		QString sql = QString("insert into TUserInfo(userName, password, savePsw, createTime, lastLoginTime, LoginCount) values('%1', '%2', %3, '%4', '%5', '%6')")
			.arg(userName)
			.arg(psw)
			.arg(0)
			.arg(strCurrentTime)
			.arg(strCurrentTime)
			.arg(0);

		return exec(sql);
	}
	return false;
}

bool UserMgr::exists(const QString &userName)
{
	QSqlQuery query(QString("select id from TUserInfo where userName = '%1'").arg(userName), *db);
	return query.next();
}

bool UserMgr::getUserInfo(QMap<QString, QVariant> &ui,const QString &userName) // 获取用户相关的信息
{
	ui.clear();
	if (exists(userName))
	{
		QSqlQuery query(QString("select * from TUserInfo where userName = '%1'").arg(userName), *db);
		query.next();
		for (int i = 0; i < query.record().count(); i++)
		{
			ui[query.record().fieldName(i)] = query.record().value(i).toString();
		}
		return true;
	}
	else
	{
		DBG(qDebug() << QString("[%1]用户不存在").arg(userName));
		return false;
	}
}

QVariant UserMgr::getUserInfo(const QString &itemName, const QString &userName) // 获取用户相关的信息
{
	if (exists(userName))
	{
		QSqlQuery query(QString("select * from TUserInfo where userName = '%1'").arg(userName), *db);
		query.next();
		return query.record().value(itemName);
	}
    return 0;
}

bool UserMgr::verifyPsw(const QString &userName,const QString &psw)
{
	QMap<QString, QVariant> ui;
	if (getUserInfo(ui, userName))
	{
		if (ui["password"]== psw)
			return true;
	}

	return false;
}

bool UserMgr::updateLoginTime(const QString &userName)
{
	QString sql = QString("update TUserInfo set lastLoginTime = '%1' where userName = '%2'")
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
		.arg(userName);
	return exec(sql);

}

bool UserMgr::updateLoginCount(const QString &userName)
{
	QString sql = QString("update TUserInfo set LoginCount = LoginCount +1 where userName = '%1'")
		.arg(userName);
	return exec(sql);
}

QStringList UserMgr::getUserList()
{
	QStringList slResult;
	QSqlQuery query("select userName from TUserInfo order by lastLoginTime desc", *db);
	while (query.next())
	{
		slResult << query.record().value("userName").toString();
	}

	return slResult;
}

QStringList UserMgr::getUserListOrderById()
{
	QStringList slResult;
	QSqlQuery query("select userName from TUserInfo order by id desc", *db);
	while (query.next())
	{
		slResult << query.record().value("userName").toString();
	}

	return slResult;

}

bool UserMgr::updateSavePsw(const QString &userName, bool bSave) // 更新保存密码
{
	QString sql = QString("update TUserInfo set savePsw = '%1' where userName = '%2'")
		.arg(QString::number(bSave))
		.arg(userName);
	return exec(sql);
}

bool UserMgr::updatePassword(const QString &userName, const QString &password)
{
	QString sql = QString("update TUserInfo set password = '%1' where userName = '%2'")
		.arg(password)
		.arg(userName);
	return exec(sql);
}


bool UserMgr::removeUser(const QString &userName)
{
	if(exists(userName))
	{
		QString sql = QString("delete from TUserInfo where userName = '%1'")
			.arg(userName);
		return exec(sql);
	}

	return false;
}
