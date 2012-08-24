#ifndef OBCDBMODEL_H
#define OBCDBMODEL_H

#include "BCDBModel.h"

class OBCDBModel : public BCDBModel
{
	Q_OBJECT

public:
	OBCDBModel(QObject *parent = 0);
	~OBCDBModel();

	virtual QVariant	data ( const QModelIndex & item, int role = Qt::DisplayRole ) const;
private:
	
};

#endif // OBCDBMODEL_H
