#pragma once

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
class LISqlQueryModel :
	public QSqlQueryModel
{
public:
	LISqlQueryModel(QObject *parent = NULL);
	virtual ~LISqlQueryModel(void);

	virtual void	sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
	void setSql(const QString &sql, const QSqlDatabase & db);
	void update();
	QVariant getValueByItemname(const QString &itemName, int row);
	virtual void resetCustomInfo() {};
	int getRowCount();
protected:
	QString sqlCmd;
	QString sortCmd;
	QSqlQuery squery;
	const QSqlDatabase *pdb;
};
