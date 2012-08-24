#ifndef GLOBAL_H
#define GLOBAL_H
#include <algorithm>


class Global
{
public:
    static QString encrypt(QString data);
    static QString uncrypt(QString data);
    static QString makePathStr(const QString &fileName);	// 生成一个路径 相对于exe所在路径,未在后面加了"\"
    static void mkPath(const QString &fileName);	// 建立目录,如果目录不存在
    static void sleep(int secs);
    static void setCodec(char *codeName);
	static void createRelExeDir(const QString &path);
	static QStringList getDirFileListByFilter(QString path, QString filter); // path为绝对路径,并要以正确的'\'结尾
	static bool saveToDisk(const QString &filename, QIODevice *data);	// 保存数据到文件
	static bool saveToDisk(const QString &filename, QByteArray &data);
	static bool deleteDir(const QString &dirName);	// 删除文件夹(包括非空)
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
