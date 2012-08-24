#ifndef PASGSOVOICEGOOGLE_H
#define PASGSOVOICEGOOGLE_H


#include "PageSOVoiceBase.h"
class PageSOVoiceGoogle : public PageSOVoiceBase
{
	Q_OBJECT

public:
	PageSOVoiceGoogle(QWidget *parent);
	~PageSOVoiceGoogle();

	virtual void save();
private:
	
};

#endif // PASGSOVOICEGOOGLE_H
