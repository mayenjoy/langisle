#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QtGui>

class DlgLogin : public QDialog
{
	Q_OBJECT

public:
	DlgLogin(QWidget *parent, bool bDisableAutoLogin);
	~DlgLogin();

	QString currentUser()
	{
		return comboBoxUserList->currentText();
	}
	virtual void	reject ();
public slots:
	void userChanged(QString userName);
	void loginClicked();
	void newUserClicked();
	void rememberPswClicked(bool checked);
	void autoLoginClicked(bool checked);
	
private:
	bool initComboBoxUserList();
private:
	QComboBox *comboBoxUserList;
	QLineEdit *lineEditPassword;
	QCheckBox *checkBoxRememberPsw;
	QCheckBox *checkBoxAutoLogin;
	bool m_bDisableAutoLogin;
};

#endif // DLGLOGIN_H
