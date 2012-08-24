#ifndef BCDBMODEL_H
#define BCDBMODEL_H

#include "../comm/LISqlQueryModel.h"
#include <QSqlField>

class BCDBModel : public LISqlQueryModel
{
	Q_OBJECT

public:
	BCDBModel(QObject *parent = 0);
	~BCDBModel();

	int getRowByBID(int bid);
	void updateHeaderName();
	QString getHeaderNameByColname(QString colname);
	virtual QVariant	data ( const QModelIndex & item, int role = Qt::DisplayRole ) const;
	static void initC2hmap();
	static void initN2size();
	void setHoverRow(int row) {hoverRow = row;}
	int getSizeSection(const QString &sname);
	QVariant getRecordByRow(int row, const QString &colName);

private:
	static QMap<QString, QString> c2hmap;
	static QMap<QString, int> n2size;
	int hoverRow;
};

#endif // BCDBMODEL_H
