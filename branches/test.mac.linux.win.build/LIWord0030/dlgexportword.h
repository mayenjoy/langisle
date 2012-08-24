#ifndef DLGEXPORTWORD_H
#define DLGEXPORTWORD_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

class DlgExportWord : public QDialog
{
	Q_OBJECT

public:
	DlgExportWord(QWidget *parent = 0);
	~DlgExportWord();

public slots:
	void browerPath();
	void ok();
private:
	QLineEdit *lineExportFilePath;
	QComboBox *comboBoxType;
};

#endif // DLGEXPORTWORD_H
