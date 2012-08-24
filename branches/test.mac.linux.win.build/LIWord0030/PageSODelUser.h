#ifndef PAGESODELUSER_H
#define PAGESODELUSER_H

#include <QDialog>
#include <QLineEdit>

class PageSODelUser : public QDialog
{
	Q_OBJECT

public:
	PageSODelUser(QWidget *parent,const QString &username);
	~PageSODelUser();

public slots:
	void delClicked();
private:
	QLineEdit *lineEditUserName;
	QLineEdit *lineEditPassword;
	QString username;

};

#endif // PAGESODELUSER_H
