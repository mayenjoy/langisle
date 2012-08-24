#pragma once

#include "UDataDB.h"
#include <QDateTime>

enum EnumTypeJudge
{
	ETJNone = 0,
	ETJKnowwell= 1, // 掌握
	ETJPlain = 2,    // 记得
	ETJDim = 3,      // 模糊
	ETJOblivious = 4,    // 忘记
	ETJUnknown = 5       // 未知
};

enum EnumTypeAffirm
{
	ETANone = 0,
	ETAYes = 1, // 正确
	ETANo = 2    // 错误
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

	int getSTime() // 本次学习用时
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

	int tscount; // 总学习数
	int nscount; // 新学单词
	int rvcount; // 复习单词
	int kncount; // 掌握单词

	QDateTime   startTime; // 开始学习时间
	EnumGWOrder m_GWOrder; // 取词顺序
	EnumREVWOrder m_RVWOrder; // 复习取词顺序
	NextWordType m_nwt; // 当前单词类型,新学,复习
};
