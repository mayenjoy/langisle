#ifndef WORDVIEWBCDBMODEL_H
#define WORDVIEWBCDBMODEL_H

#include "BCDBModel.h"

class WordViewBCDBModel : public BCDBModel
{
	Q_OBJECT

public:
	WordViewBCDBModel(QObject *parent = 0);
	~WordViewBCDBModel();

	virtual QVariant	data ( const QModelIndex & item, int role = Qt::DisplayRole ) const;
private:
	
};

#endif // WORDVIEWBCDBMODEL_H
