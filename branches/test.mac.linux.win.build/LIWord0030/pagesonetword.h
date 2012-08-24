#ifndef PAGESONETWORD_H
#define PAGESONETWORD_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
class PageSONetword : public QWidget
{
	Q_OBJECT

public:
	PageSONetword(QWidget *parent);
	~PageSONetword();

	void save();
	void initData();
public slots:
	void testClick();
	void saveClick();
private:
	QComboBox *type;
	QLineEdit *leAddr;
	QLineEdit *lePort;
	QLineEdit *leUName;
	QLineEdit *lePWord;
};

#endif // PAGESONETWORD_H
