#include "stdafx.h"
#include "LMFree.h"

LMFree::LMFree(UDataDB *puddb) : LMBase(puddb)
{
}

LMFree::~LMFree(void)
{
} 

int LMFree::getNextWord()
{
	//DBG(QDateTime   start   =   QDateTime::currentDateTime());

	int mid = pUDDB->getWantReviewMID(m_RVWOrder);
	if (mid <= 0)
	{
		mid = pUDDB->getNotLearnMID(m_GWOrder);
		if (mid <= 0)
		{
			if (pUDDB->getLEThanStCount(pUDDB->getMaxFP()) <= 0)
			{
				return -7;
			}
			else
			{
				return mid;
			} 
		}
		else
		{
			DBG(qDebug() << "新学单词" << mid);
			m_nwt = NWTNew;
		}
	}
	else
	{
		m_nwt = NWTREView;
		DBG(qDebug() << "复习单词" << mid);
	}
	//DBG(qDebug() << "取新词mid耗时" << start.msecsTo(QDateTime::currentDateTime()));

	
	m_wi.clear();

	//DBG(start   =   QDateTime::currentDateTime());
	m_wi = pUDDB->getWordInfoByMid(mid);



	//DBG(qDebug() << "取新词信息耗时" << start.msecsTo(QDateTime::currentDateTime()));
	// pUDDB->updateWordMInfo(m_wi);
	// 	DBG(qDebug() << "LMFree::getNextWord" << mid);
	// 	pUDDB->updateWordMInfo(m_wi);
	return mid;

}

// 10分制
void LMFree::updateMInfo(QMap<QString, QVariant> &tval)
{
	tscount++;
	switch (m_nwt)
	{
	case NWTNew:
		nscount++;
		break;
	case NWTREView:
		rvcount++;
		break;

	}
	switch (tval["selJudge"].toInt())
	{
	case ETJNone:
		break;
	case ETJKnowwell: // 掌握
		m_wi["st"] = pUDDB->getMaxFP();
		kncount++;
		break;
	case ETJPlain:    // 记得
		{
			switch (tval["selAffirm"].toInt())
			{
			case 	ETANone:
				break;
			case ETAYes: // 记得->正确
				{
					if (m_wi["sc"].toInt() == 0) // 首次就记得正确
					{
						m_wi["st"] = pUDDB->getMaxFP();
						kncount++;
					}
					else
					{
						m_wi["st"] = pUDDB->getNextFP(m_wi["st"].toInt());
						m_wi["score"] = m_wi["score"].toInt() + 10;
						if (pUDDB->getMaxFP() == m_wi["st"]) // 等于最长遗忘点,记为掌握
						{
							kncount++;
						}
					}
				}
				break;
			case ETANo:    // 记得->错误
				{
					m_wi["st"] = pUDDB->getNextFP(0); // 回到起点
					m_wi["score"] = m_wi["score"].toInt() - 10;
				}
				break;
			}
		}
		break;
	case ETJDim:      // 模糊
		switch (tval["selAffirm"].toInt())
		{
		case 	ETANone:
			break;
		case ETAYes: // 模糊->正确
			{
				m_wi["st"] = pUDDB->getNextFP(m_wi["st"].toInt()) - m_wi["st"].toInt(); // 比 记得 正确 提前一点复习
				m_wi["score"] = m_wi["score"].toInt() + 5;
			}
			break;
		case ETANo:    // 模糊->错误
			{
				m_wi["st"] = pUDDB->getNextFP(0); // 回到起点
				m_wi["score"] = m_wi["score"].toInt() - 5;
			}
			break;
		}
		break;
	case ETJOblivious:    // 忘记
		{
			m_wi["st"] = pUDDB->getNextFP(0); // 回到起点
			m_wi["score"] = m_wi["score"].toInt() - 20;
		}
		break;
	case ETJUnknown:       // 未知
		{
			m_wi["st"] = pUDDB->getNextFP(0); // 回到起点
			m_wi["score"] = 0;
		}
		break;
	}
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	pUDDB->updateWordMInfo(m_wi);

	// 这里耗时较多,要整理到内存表,关闭时再写入
	DBG(qDebug() << "updateMInfo 1 耗时" << start.msecsTo(QDateTime::currentDateTime()));
	pUDDB->updateWordLearnLog(tval);
	DBG(qDebug() << "updateMInfo 2 耗时" << start.msecsTo(QDateTime::currentDateTime()));
}
