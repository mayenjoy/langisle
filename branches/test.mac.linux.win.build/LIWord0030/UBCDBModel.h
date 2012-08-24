#ifndef UBCDBMODEL_H
#define UBCDBMODEL_H

#include "BCDBModel.h"

class UBCDBModel : public BCDBModel
{
	Q_OBJECT

public:
	UBCDBModel(QObject *parent = 0);
	~UBCDBModel();

	virtual QVariant	data ( const QModelIndex & item, int role = Qt::DisplayRole ) const;
private:
	
};

#endif // UBCDBMODEL_H
