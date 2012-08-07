#ifndef PAGESOVTTS_H
#define PAGESOVTTS_H

#include "PageSOVoiceBase.h"
#include <QtGui>

class PageSOVoiceTTS : public PageSOVoiceBase
{
	Q_OBJECT

public:
	PageSOVoiceTTS(QWidget *parent);
	~PageSOVoiceTTS();

	QString currentVoiceName();
	void save();
private slots:
	void currentIndexChanged(const QString & text);
private:
	QComboBox *comboVoiceList;	
};

#endif // PAGESOVTTS_H
