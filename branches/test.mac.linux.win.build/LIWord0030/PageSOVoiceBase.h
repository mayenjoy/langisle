#ifndef PAGESOVOICEBASE_H
#define PAGESOVOICEBASE_H

#include <QWidget>

class PageSOVoiceBase : public QWidget
{
	Q_OBJECT

public:
	PageSOVoiceBase(QWidget *parent);
	~PageSOVoiceBase();

	virtual void save() = 0;
private:
	
};

#endif // PAGESOVOICEBASE_H
