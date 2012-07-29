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
			DBG(qDebug() << "��ѧ����" << mid);
			m_nwt = NWTNew;
		}
	}
	else
	{
		m_nwt = NWTREView;
		DBG(qDebug() << "��ϰ����" << mid);
	}
	//DBG(qDebug() << "ȡ�´�mid��ʱ" << start.msecsTo(QDateTime::currentDateTime()));

	
	m_wi.clear();

	//DBG(start   =   QDateTime::currentDateTime());
	m_wi = pUDDB->getWordInfoByMid(mid);



	//DBG(qDebug() << "ȡ�´���Ϣ��ʱ" << start.msecsTo(QDateTime::currentDateTime()));
	// pUDDB->updateWordMInfo(m_wi);
	// 	DBG(qDebug() << "LMFree::getNextWord" << mid);
	// 	pUDDB->updateWordMInfo(m_wi);
	return mid;

}

// 10����
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
	case ETJKnowwell: // ����
		m_wi["st"] = pUDDB->getMaxFP();
		kncount++;
		break;
	case ETJPlain:    // �ǵ�
		{
			switch (tval["selAffirm"].toInt())
			{
			case 	ETANone:
				break;
			case ETAYes: // �ǵ�->��ȷ
				{
					if (m_wi["sc"].toInt() == 0) // �״ξͼǵ���ȷ
					{
						m_wi["st"] = pUDDB->getMaxFP();
						kncount++;
					}
					else
					{
						m_wi["st"] = pUDDB->getNextFP(m_wi["st"].toInt());
						m_wi["score"] = m_wi["score"].toInt() + 10;
						if (pUDDB->getMaxFP() == m_wi["st"]) // �����������,��Ϊ����
						{
							kncount++;
						}
					}
				}
				break;
			case ETANo:    // �ǵ�->����
				{
					m_wi["st"] = pUDDB->getNextFP(0); // �ص����
					m_wi["score"] = m_wi["score"].toInt() - 10;
				}
				break;
			}
		}
		break;
	case ETJDim:      // ģ��
		switch (tval["selAffirm"].toInt())
		{
		case 	ETANone:
			break;
		case ETAYes: // ģ��->��ȷ
			{
				m_wi["st"] = pUDDB->getNextFP(m_wi["st"].toInt()) - m_wi["st"].toInt(); // �� �ǵ� ��ȷ ��ǰһ�㸴ϰ
				m_wi["score"] = m_wi["score"].toInt() + 5;
			}
			break;
		case ETANo:    // ģ��->����
			{
				m_wi["st"] = pUDDB->getNextFP(0); // �ص����
				m_wi["score"] = m_wi["score"].toInt() - 5;
			}
			break;
		}
		break;
	case ETJOblivious:    // ����
		{
			m_wi["st"] = pUDDB->getNextFP(0); // �ص����
			m_wi["score"] = m_wi["score"].toInt() - 20;
		}
		break;
	case ETJUnknown:       // δ֪
		{
			m_wi["st"] = pUDDB->getNextFP(0); // �ص����
			m_wi["score"] = 0;
		}
		break;
	}
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	pUDDB->updateWordMInfo(m_wi);

	// �����ʱ�϶�,Ҫ�����ڴ��,�ر�ʱ��д��
	DBG(qDebug() << "updateMInfo 1 ��ʱ" << start.msecsTo(QDateTime::currentDateTime()));
	pUDDB->updateWordLearnLog(tval);
	DBG(qDebug() << "updateMInfo 2 ��ʱ" << start.msecsTo(QDateTime::currentDateTime()));
}
