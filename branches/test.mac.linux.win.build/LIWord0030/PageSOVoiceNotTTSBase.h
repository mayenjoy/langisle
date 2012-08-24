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
	QCheckBox *checkBoxOnFailUserTTS;	// ������ʧ��ʱ,ʹ��TTS
	QVBoxLayout *mainLayout;
};

#endif // PASGSOVOICENOTTTSBASE_H
