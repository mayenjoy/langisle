#ifndef PAGESOGENERAL_H
#define PAGESOGENERAL_H

#include <QtGui>
#include "../comm/UpdateMgr.h"
class PageSOGeneral : public QWidget
{
	Q_OBJECT

public:
	PageSOGeneral(QWidget *parent);
	~PageSOGeneral();

	void save();
private slots:
	void checkUpdate();
	void checkResult(bool bHaveNewVer);
	void	stateChanged ( int state );
	void enableTraystateChanged( int state );
private:
	UpdateMgr m_um;
	QPushButton *checkUpdateButton;
};

#endif // PAGESOGENERAL_H
