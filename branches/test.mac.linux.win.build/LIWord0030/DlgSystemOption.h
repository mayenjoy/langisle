#ifndef DLGSYSTEMOPTION_H
#define DLGSYSTEMOPTION_H

#include <QDialog>

class PageSOVoice;
class PageSOGeneral;
class PageSOPicture;
class PageSODict;
class PageSOAdvanced;
class PageSORemind;
//wuling
class PageSOUser;
class PageSONetword;
class DlgSystemOption : public QDialog
{
	Q_OBJECT

public:
	DlgSystemOption(QWidget *parent);
	~DlgSystemOption();
private slots:
	void okClicked();
private:
	PageSOGeneral *psog;
	PageSOVoice *psov;
	PageSOPicture *psop;
	PageSODict *psod;
	PageSOAdvanced *psoa;
	PageSORemind *psor;
	//wuling
	PageSOUser *psou;

	PageSONetword *psnw;
};

#endif // DLGSYSTEMOPTION_H
