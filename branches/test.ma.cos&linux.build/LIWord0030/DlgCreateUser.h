#ifndef DLGCREATEUSER_H
#define DLGCREATEUSER_H

#include <QDialog>
#include <QLineEdit>

class DlgCreateUser : public QDialog
{
	Q_OBJECT

public:
	DlgCreateUser(QWidget *parent);
	~DlgCreateUser();

public slots:
	void createClicked();
private:
	QLineEdit *lineEditUserName;
	QLineEdit *lineEditPassword;
	QLineEdit *lineEditVerifyPsw;

};

#endif // DLGCREATEUSER_H
