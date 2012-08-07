#pragma once

#include "UDataDB.h"
#include <QDateTime>

enum EnumTypeJudge
{
	ETJNone = 0,
	ETJKnowwell= 1, // ����
	ETJPlain = 2,    // �ǵ�
	ETJDim = 3,      // ģ��
	ETJOblivious = 4,    // ����
	ETJUnknown = 5       // δ֪
};

enum EnumTypeAffirm
{
	ETANone = 0,
	ETAYes = 1, // ��ȷ
	ETANo = 2    // ����
};

enum NextWordType 
{
	NWTNew = 1,
	NWTREView = 2
};

class LMBase
{
public:
	LMBase(UDataDB *puddb);
	virtual ~LMBase(void);

	virtual int getNextWord() = 0;
	void setWordInfo(int mid)
	{
		m_wi.clear();
		m_wi = pUDDB->getWordInfoByMid(mid);
	}
	WordInfo &wordInfo() {return m_wi;};
	virtual	void updateMInfo(QMap<QString, QVariant> &tval) = 0;

	int getSTime() // ����ѧϰ��ʱ
	{
		return startTime.msecsTo(QDateTime::currentDateTime());
	}
	int getTSCount() {return tscount;};
	int getNSCount() {return nscount;};
	int getRVCount() {return rvcount;};
	int getKNCount() {return kncount;};
	int getSSpeed() {return  tscount / (getSTime() / (60 * 60 * 1000.0));};
	void setGWGWOrder(EnumGWOrder GWOrder) {m_GWOrder = GWOrder;};
	void setRVWOrder(EnumREVWOrder RVWOrder) {m_RVWOrder = RVWOrder;};
	QDateTime getStartTime() {return startTime;};
protected:
	UDataDB *pUDDB;
	WordInfo m_wi;

	int tscount; // ��ѧϰ��
	int nscount; // ��ѧ����
	int rvcount; // ��ϰ����
	int kncount; // ���յ���

	QDateTime   startTime; // ��ʼѧϰʱ��
	EnumGWOrder m_GWOrder; // ȡ��˳��
	EnumREVWOrder m_RVWOrder; // ��ϰȡ��˳��
	NextWordType m_nwt; // ��ǰ��������,��ѧ,��ϰ
};
