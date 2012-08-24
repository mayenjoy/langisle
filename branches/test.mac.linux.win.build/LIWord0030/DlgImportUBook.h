#ifndef DLGIMPORTUBOOK_H
#define DLGIMPORTUBOOK_H

#include <QtGui>
#include "BookConvertMgr.h"
class BookDB;
class DlgImportUBook : public QDialog
{
	Q_OBJECT

public:
	DlgImportUBook(QWidget *parent, BookDB *bdb);
	~DlgImportUBook();
public slots:
	void importFile();
	void ok();
private:
	BookDB *mbdb;
	QLineEdit *lineImportFilePath;
	QComboBox *comboBoxType;
	BookConvertMgr bcm;
};

#endif // DLGIMPORTUBOOK_H
