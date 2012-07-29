#ifndef GLOBAL_H
#define GLOBAL_H
#include <algorithm>


class Global
{
public:
    static QString encrypt(QString data);
    static QString uncrypt(QString data);
    static QString makePathStr(const QString &fileName);	// ����һ��·�� �����exe����·��,δ�ں������"\"
    static void mkPath(const QString &fileName);	// ����Ŀ¼,���Ŀ¼������
    static void sleep(int secs);
    static void setCodec(char *codeName);
	static void createRelExeDir(const QString &path);
	static QStringList getDirFileListByFilter(QString path, QString filter); // pathΪ����·��,��Ҫ����ȷ��'\'��β
	static bool saveToDisk(const QString &filename, QIODevice *data);	// �������ݵ��ļ�
	static bool saveToDisk(const QString &filename, QByteArray &data);
	static bool deleteDir(const QString &dirName);	// ɾ���ļ���(�����ǿ�)
#ifdef __WINDOWS
	static void deleteDirWin(LPCWSTR lpDir);
#endif
	static bool copyDir(const QString &source, const QString &destination, const bool override = true) ;
	static QString makeRelativeFilePath(const QString &fileName);
	template <typename T> 
	QList<T> static reversed( const QList<T> & in ) 
	{    
		QList<T> result;     
		std::reverse_copy( in.begin(), in.end(), std::back_inserter( result ) );     
		return result; 
	};

};

#endif // GLOBAL_H
