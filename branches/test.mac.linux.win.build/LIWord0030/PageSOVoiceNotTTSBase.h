#ifndef PASGSOVOICENOTTTSBASE_H
#define PASGSOVOICENOTTTSBASE_H

#include "PageSOVoiceBase.h"
#include <QtGui>

class PageSOVoiceNotTTSBase : public PageSOVoiceBase
{
	Q_OBJECT

public:
	PageSOVoiceNotTTSBase(QWidget *parent);
	~PageSOVoiceNotTTSBase();

	virtual void save() = 0;
protected:
	QCheckBox *checkBoxOnFailUserTTS;	// 当发音失败时,使用TTS
	QVBoxLayout *mainLayout;
};

#endif // PASGSOVOICENOTTTSBASE_H
