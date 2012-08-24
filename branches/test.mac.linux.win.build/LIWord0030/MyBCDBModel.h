#ifndef MYBCDBMODEL_H
#define MYBCDBMODEL_H

#include "BCDBModel.h"

class MyBCDBModel : public BCDBModel
{
	Q_OBJECT

public:
	MyBCDBModel(QObject *parent = 0);
	~MyBCDBModel();

	virtual QVariant	data ( const QModelIndex & item, int role = Qt::DisplayRole ) const;
	virtual void resetCustomInfo();
private:
        QList< QMap<QString, QVariant> > lcinfo;
	int count;
};

#endif // MYBCDBMODEL_H
