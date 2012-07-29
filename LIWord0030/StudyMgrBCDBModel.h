#ifndef STUDYMGRBCDBMODEL_H
#define STUDYMGRBCDBMODEL_H

#include "BCDBModel.h"

class StudyMgrBCDBModel : public BCDBModel
{
		Q_OBJECT
public:
	StudyMgrBCDBModel(QObject *parent = 0);
	~StudyMgrBCDBModel();
	virtual QVariant	data ( const QModelIndex & item, int role = Qt::DisplayRole ) const;
	void sort(int column, Qt::SortOrder order);

	
};
#endif // STUDYMGRBCDBMODEL_H