#ifndef PAGESMSTATUS_H
#define PAGESMSTATUS_H

#include <QtGui>

class PageSMStatus : public QDialog
{
		Q_OBJECT
public:
	PageSMStatus(QWidget *parent,int mwid,const QString &word,const QString &status);
	~PageSMStatus();

private:
	bool initComboBoxStatusList();

public slots:
		void modifyClicked();

private:
	QComboBox *comboBoxStatusList;
	QLineEdit *lineEditWord;
	QLineEdit *lineEditOldStatus;

	QString word;
	QString status;
	int mmid;
};

#endif // PAGESMSTATUS_H