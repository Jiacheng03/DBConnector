/****************************************************
*		class DBConnector
*		��MySQL�ṩ��C api����װ
*****************************************************/

#pragma once

#ifdef _MSC_VER
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

class Result;
typedef shared_ptr<Result> ResultSet;

//================================= DBConnector ======================================//
class DBConnector
{
public:
	// ��Ա��ʼ��
	DBConnector(string host, string user, string passwd, string dbname, unsigned int port, string charset = "utf8");
	
	// ����MYSQL����
	~DBConnector();

	// ��ʼ��MYSQL��������mysql. ��ʧ�ܣ��򷵻�false.
	bool Initialize();

	// ִ�в�ѯ���, ����Resultָ�룬����ʹ��ResultSet����д���
	// ������Ϊ�գ��򷵻�NULL���������Ϊ��
	Result* Query(string sql);

	// ִ�и���/����/ɾ�����. ��ʧ�ܣ��򷵻�false.
	bool Update(string sql);

	// ��ȡ������
	unsigned int GetErrno();

	// ��ȡ������Ϣ
	const char* GetErrMsg();	

private:
	string m_host;
	string m_user;
	string m_passwd;
	string m_dbname;
	unsigned int m_port;
	string m_charset;

	MYSQL m_dbConn;
	unsigned int m_errno;
	string m_error;
};

//======================================= Result ===============================================//
class Result
{
public:
	// ��¼������и��ֶε�˳��
	Result(MYSQL_RES* res);

	// ���������
	~Result();

	// �������Ϊ�գ��򷵻�true
	bool IsEmpty();

	// ���ؽ������������
	my_ulonglong GetRowCount();

	// ���α��Ƶ���һ��
	bool FetchOne();

	// ��ȡ��ǰ���У����ֶζ�Ӧ��ֵ����const char*�ͷ���
	const char* GetStr(string fidname);

	// ��ȡ��ǰ���У����ֶζ�Ӧ��ֵ����int�ͷ���
	int GetInt(string fidname);

	// ��ȡ��ǰ���У����ֶζ�Ӧ��ֵ����double�ͷ���
	double GetDouble(string fidname);

private:
	MYSQL_RES * m_res;
	MYSQL_ROW m_row;
	unordered_map<string, int> m_fidmap;	
};