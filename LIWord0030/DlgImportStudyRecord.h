#ifndef DLGIMPORTSTUDYRECORD_H
#define DLGIMPORTSTUDYRECORD_H

#include <QtGui>

#include "ImportStudyRecordMgr.h"

class DlgImportStudyRecord : public QDialog
{
	Q_OBJECT

public:
	DlgImportStudyRecord(QWidget *parent);
	~DlgImportStudyRecord();
public slots: 
	void browerPath();
	void ok();
private:
	QLineEdit *lineImportFilePath;
	QComboBox *comboBoxType;

	ImportStudyRecordMgr isrm;
};

#endif // DLGIMPORTSTUDYRECORD_H
