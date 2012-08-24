#ifndef PAGESOADDUSER_H
#define PAGESOADDUSER_H

#include <QDialog>
#include <QLineEdit>

class PageSOAddUser : public QDialog
{
	Q_OBJECT

public:
	PageSOAddUser(QWidget *parent);
	~PageSOAddUser();

public slots:
	void createClicked();
private:
	QLineEdit *lineEditUserName;
	QLineEdit *lineEditPassword;
	QLineEdit *lineEditVerifyPsw;

};

#endif // PAGESOADDUSER_H
