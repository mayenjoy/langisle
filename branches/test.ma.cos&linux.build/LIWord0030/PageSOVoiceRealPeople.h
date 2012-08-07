#ifndef PAGESOVOICEREALPEOPLE_H
#define PAGESOVOICEREALPEOPLE_H

#include "PageSOVoiceNotTTSBase.h"

class PageSOVoiceRealPeople : public PageSOVoiceNotTTSBase
{
	Q_OBJECT

public:
	PageSOVoiceRealPeople(QWidget *parent);
	~PageSOVoiceRealPeople();

	virtual void save();
private slots:
	void browerPath();
private:
	QLineEdit *editPath;
};

#endif // PAGESOVOICEREALPEOPLE_H
