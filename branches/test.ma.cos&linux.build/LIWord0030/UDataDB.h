#pragma once
#include "../comm/LISqlite.h"

#include "config.h"
#include "MBCDBMgr.h"
#include <QDateTime>

enum EnumGWOrder 
{
	EGWabc = 0, // ˳��
	EGWcba = 1, // ����
	EGWbac = 2, // ����
};

enum EnumREVWOrder // ��ϰ����˳��
{
	RVWabc = 0, // ˳��
	RVWbac = 2, // ����
	
};

class UDataDB :
	public LISqlite
{
public:
	UDataDB(const QString &dbpath, const QString &connName);
	virtual ~UDataDB(void);

	void init();
	void uninit();

	void loadDict(int dbname);//��ӹ������ݿ�

	void importStudyInfo(QList<BOOKINFO> &bil);
	void importStudyInfo(BOOKINFO &bi);

	//////////////////////////////////////////////////////////////////////////
	// ר��v02X
	void updateMDataFrom02XSData(QList<WordInfo> &wil);
	int getMidByDictWord(const QString &word);
	int getMidByUWord(const QString &word);
	void updateWordMInfo2(WordInfo &winfo);
	//////////////////////////////////////////////////////////////////////////
	WordInfo getWordInfoByMid(int mid);
	QString getCurrentBookInfo(); // ��ȡ��ǰ�α���Ϣ,������ʾ
	int getWantReviewMID(); // ��ȡ��Ҫ��ϰ�ĵ���id,û�з���0
	int getWantReviewMID(EnumREVWOrder RVWOrder); // ��ȡ��Ҫ��ϰ�ĵ���id,û�з���0
	int getNotLearnMID(EnumGWOrder GWRandom = EGWabc); // ��ȡδѧϰ�ĵ���id,û�з���0
	int getStudyDataWordCount();	// ��ȡ��ǰѧϰ�ĵ�������
	void updateWordMInfo(WordInfo &winfo);	// ���µ��ʼ�����Ϣ
	void setConfigInfo(const QString &name, const QString &value);
	QString getConfigInfo(const QString &name);
	int getNextFP(int CFP); // CFP��ǰ��������,��һ��������
	double getNowFP(int CFP); // ��ȡst���������е�λ��,��ѧϰ����
	int getMaxFP(); // ��ȡ����������

	void updateWordLearnLog(QMap<QString, QVariant> &tval);

	int getGreaterThanStCount(int st);	// ��ȡʱ�������ڸ���ֵ�ĵ��ʸ���
	int getLEThanStCount(int st); // ��ȡʱ����С�ڵ��ڸ���ֵ�ĵ��ʸ���
	EnumMyCategory getBookCid(const QString &bookTableName, int maxst); // ���������������ж�ĳ���ʿ���ѧϰ��,����������
	void updateMBCid(int bid, bookType bt, int cid); // �����ҵĴʿ�cid


	int getWcount(const QString &bookTableName);//��������
	//void updateWcount(int bid, bookType bt, int scount);

	int getSCount(const QString &bookTableName); // ��ѧ������
	void updateSCount(int bid, bookType bt, int scount);

	int getKNCount(const QString &bookTableName, int maxst); // �����յ�����
	void updateKNCount(int bid, bookType bt, int kncount);

	void updateProgress(int bid, bookType bt, float progress);
	float getProgress(const QString &bookTableName,int maxst);
	void updateStudyProgress(); // ����ѧϰ������Ϣ
	void updateBookStudyProgress(int bid, bookType bt); // ����ѧϰ������Ϣ

	void updateStudyDBInfo(int bid, bookType bt);	// �û��γ̼��뵥�ʺ�,ѧϰ���Ĵʿ���Ҫ����ѧϰ��Ϣ,ͬ���޸ĺ�Ĵʿ�

	bool isStudying(int bid, bookType bt);	// �ÿγ�����ѧϰ����
	void deleteBookData(const QString &tableName);
	void deleteStudyDBInfo(int bid, bookType bt, bool bClearBookStudyInfo);

	int getBookReviewCount(int bid, bookType bt); // �����γ̴���ϰ������,-1ʧ��
	int getReviewCount();	// ��ǰ��Ҫ��ϰ�ĵ�����

	void initFP(); // ��ʼ��������

	QDateTime getMaxLst(); // ���ѧϰʱ��

	void getUserBookBookListInfo(QList<QVariant> &info); // �û��ʿ��б�

	int getWidByDictWord(const QString &word);
	WordInfo getWordInfoByWid(int wid);

	// �������ĵ��� ��Ҫʵ��һ��������,�ڼ������ɾ�����пγ��ж�û�еĵ���
	void cleanSuspendWord();

	//���һ�����ʵ�ѧϰ��Ϣ
	void clearBookStudyInfo(int mid);
	//��һ������ѧϰ״̬�ĳ�����
	void updateBookStudyInfoknown(int mid);

	// ����һ��ѧϰ��־��¼
	void insertTLog(QMap<QString, QVariant> &tlog); 

	int getTodaySWCount();	// ����ѧϰ������
	int getTodaySNWCount();	// ������ѧ������
	int getTodayRSCount();	// ���츴ϰ������
	int getTodayKNCount();	// �������յ�����
	int getTodaySTime();	// ����ѧϰʱ��

protected:
	

	void createTable();
	void createBookTable(const QString &tableName); // �����α���
	void CreateGroupTable(const QString &tableName); // ���������
	void importBookData(const QString &tableName, const QString &bookDBPath); // �����α�����
	void importToUWord(); // �����ٷ��ֵ��ﲻ���ڵĴ������û��ʿ��UWord
	int importToMData(wordType wtype, int wid); // ���뵥�ʵ��û������

	void insertToMyBook(BOOKINFO &bookInfo, QString &tableName);	// ����α������ҵĿα�������

	void createMemoryMDataTable();
	void insertCurrentStudyData(const QString &bookTableName);	// ���뵱ǰѧϰ����	
	void insertCurrentGroupData(const QString &groupTableName);

	void syncMData();	// ͬ��ѧϰ�ǵ��ļ����ݿ���
	QString getSens(int wid); // ��ȡ����

	QList<BOOKINFO> getAllStudyBookInfo();

	QString getTableName(int bid, bookType bt);

	// ���ø����γ�ȫ������Ϊδѧϰ״̬
	void clearBookStudyInfo(int bid, bookType bt);

	bool getAllBookTableName(QStringList &sl);
	
	void createTLogTable(); // ����ÿ��ѧϰ��־��
	void synDSLog(); // ���ڴ��ͬ��dslog

public:
	QList<QVariant> getAllWordList();	// ��ȡ����ѧϰ��������ʾ�ĵ����б�
protected:
	QList<BOOKINFO> m_biList;
	QList<int> m_fpList;
	int maxfp;
	QDateTime m_lstMax;
};
