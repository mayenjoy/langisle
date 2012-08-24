#ifndef PAGESOEDITUSER_H
#define PAGESOEDITUSER_H

#include <QDialog>
#include <QLineEdit>

class PageSOEditUser : public QDialog
{
	Q_OBJECT

public:
	PageSOEditUser(QWidget *parent,const QString &username);
	~PageSOEditUser();

	public slots:
		void editClicked();
private:
	QLineEdit *lineEditUserName;
	QLineEdit *lineEditOldPassword;
	QLineEdit *lineEditPassword;
	QLineEdit *lineEditVerifyPsw;
	QString username;

};

#endif // PAGESOEDITUSER_H