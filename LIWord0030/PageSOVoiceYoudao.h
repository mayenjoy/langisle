#ifndef PASGSOVOICEYOUDAO_H
#define PASGSOVOICEYOUDAO_H

#include "PageSOVoiceBase.h"
class PageSOVoiceYoudao : public PageSOVoiceBase
{
	Q_OBJECT

public:
	PageSOVoiceYoudao(QWidget *parent);
	~PageSOVoiceYoudao();

	virtual void save();
private:
	
};

#endif // PASGSOVOICEYOUDAO_H
