#ifndef PAGESOREMIND_H
#define PAGESOREMIND_H

#include <QWidget>

class PageSORemind : public QWidget
{
	Q_OBJECT

public:
	PageSORemind(QWidget *parent);
	~PageSORemind();

	void save();
public slots:
	void checkEnableRemindstateChanged ( int state );
	void checkEnableHightLightChanged ( int state );
	void currentIndexChangedcomboThreshold ( const QString & text );
	
private:
	
};

#endif // PAGESOREMIND_H
