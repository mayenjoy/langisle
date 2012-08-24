#ifndef DLGSTUDYMGR3_H
#define DLGSTUDYMGR3_H

#include <QDialog>
#include <QLabel>
#include "LITableView.h"
class UDataDB;
class StudyMgrModel;
class DlgStudyMgr3 : public QDialog
{
	Q_OBJECT

public:
	DlgStudyMgr3(QWidget *parent);
	virtual ~DlgStudyMgr3();

	

public slots:
	void	updateStatusSel ( const QModelIndex & index );
	void updateBookSelectInfo();
	void exportQAClick();
	void save();
private:
	LITableView *wordTable;
	UDataDB *pudb;
	StudyMgrModel *model;

	QLabel *bookSelectInfo;

private:
    void ExportToQA(QString fileName);
};

#endif // DLGSTUDYMGR3_H
