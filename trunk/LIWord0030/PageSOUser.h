#ifndef PAGESOUSER_H
#define PAGESOUSER_H

#include <QtGui/QtGui>

class PageSOUser : public QWidget
{
	Q_OBJECT

public:
	PageSOUser(QWidget *parent);
	~PageSOUser();

	void save();
	private slots:
		//void editmgrpsw();
		void add();
		void edit();
		void del();
		void itemSelectionChanged();

private:
	bool initUserList(QStringList userList);

private:
	QTableWidget *tableWidget;
	//QPushButton *editmgrpswButton;
	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *delButton;
};

#endif // PAGESOUSER_H
