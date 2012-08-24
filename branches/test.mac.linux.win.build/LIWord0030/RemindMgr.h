#pragma once
#include <QObject>

class DlgRemind;
class RemindMgr : public QObject
{
	Q_OBJECT
public:
	RemindMgr(QObject *parent);
	virtual ~RemindMgr(void);

	bool isEnableRemind() {return m_bEnableRemind;};
	void setEnableRemind(bool bEnable);

	bool isEnableHightLight() {return m_bEnableHightLight;};
	void setEnableHightLight(bool bEnable);

	int getThreshold() {return m_nThreshold;};
	void setThreshold(int nVal);

	void tempDisableRemind() {
		tempDisable = true;
	};
	void reset() {timerEventCount = 1;};
protected:
	virtual void timerEvent(QTimerEvent *);
private:
	volatile bool m_bEnableRemind;
	volatile int m_nThreshold;
	int timerID;
	volatile int timerEventCount;
	volatile int m_nCheckSpacing;
	volatile bool tempDisable;
	DlgRemind *dlgRemind;

	bool m_bEnableHightLight;
};
