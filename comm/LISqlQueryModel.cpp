#include "stdafx.h"
#include "LISqlQueryModel.h"

LISqlQueryModel::LISqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
{
}

LISqlQueryModel::~LISqlQueryModel(void)
{
}



void LISqlQueryModel::sort(int column, Qt::SortOrder order)
{
	
	sortCmd = sqlCmd + QString(" order by %1 %2")
		.arg(record().fieldName(column))
		.arg(Qt::AscendingOrder == order ? "" : "desc");
	setQuery(sortCmd, *pdb);
	resetCustomInfo();
	//qDebug() << "LISqlQueryModel::sort" << record().fieldName(column) << sortCmd;
}

void LISqlQueryModel::setSql(const QString &sql, const QSqlDatabase & db)
{
	sortCmd = sqlCmd = sql;
	pdb = &db;
	setQuery(sql, db);
	if (lastError().isValid())
		DBG(qDebug() << "LISqlQueryModel::setSql" << lastError());
	resetCustomInfo();

}

void LISqlQueryModel::update()
{
	setQuery(sortCmd, *pdb);
	resetCustomInfo();
}

QVariant LISqlQueryModel::getValueByItemname(const QString &itemName, int row)
{
	QSqlRecord r = record(row);
	if (r.isEmpty())
		return "";

	return r.value(itemName);

}

int LISqlQueryModel::getRowCount()
{
	while(canFetchMore())	// sqlite 不支持返回全部记录数,在全部读出后rowCount才会正确
		fetchMore(); 

	return rowCount();
}