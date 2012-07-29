#ifndef PAGESOVOICE_H
#define PAGESOVOICE_H

#include <QtGui>

class PageSOVoiceTTS;
class PageSOVoiceYoudao;
class PageSOVoiceGoogle;
class PageSOVoiceRealPeople;

class PageSOVoice : public QWidget
{
	Q_OBJECT

public:
	PageSOVoice(QWidget *parent);
	~PageSOVoice();

	void save();
private slots:
	void testSpeak();
private:
	QComboBox *voiceSelComboBox;
	//QCheckBox *checkBoxLoopPlay;
	QLineEdit *lineTestWord;
	QStackedWidget *stacketWidget;

	PageSOVoiceTTS *pageSOVoiceTTS;
	PageSOVoiceYoudao *pageSOVoiceYoudao;
	PageSOVoiceGoogle *pageSOVoiceGoogle;
	PageSOVoiceRealPeople *pageSOVoiceRealPeople;
};

#endif // PAGESOVOICE_H
