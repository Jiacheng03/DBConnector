#include "DBConnector.h"
#include <algorithm>

//================================= DBConnector ======================================//
// ��Ա��ʼ��
DBConnector::DBConnector(string host, string user, string passwd, string dbname, unsigned int port, string charset)
:m_host(host), m_user(user), m_passwd(passwd), m_dbname(dbname), m_port(port), m_charset(charset)
{}

// ����MYSQL����
DBConnector::~DBConnector()
{
	mysql_close(&m_dbConn);
}

// ��ʼ��MYSQL��������mysql. 
// ��ʧ�ܣ��򷵻�false.
bool DBConnector::Initialize()
{
	mysql_init(&m_dbConn);

	// ����mysql
	MYSQL * conn = mysql_real_connect(&m_dbConn, m_host.c_str(), m_user.c_str(),
		m_passwd.c_str(), m_dbname.c_str(), m_port, NULL, 0);

	// �����ӳɹ�����������ʹ�õ��ַ���
	if (conn != NULL)
	{		
		string sql = string("set names ") + m_charset;
		mysql_real_query(&m_dbConn, sql.c_str(), (unsigned long)sql.length());
	}
	else
	{		
		m_errno = mysql_errno(&m_dbConn);
		m_error = mysql_error(&m_dbConn);
	}
	
	return (conn != NULL);
}

// ִ�в�ѯ���, ����Resultָ�룬����ʹ��ResultSet����д���
// ������Ϊ�գ��򷵻�NULL���������Ϊ��
Result* DBConnector::Query(string sql)
{
	MYSQL_RES * res = NULL;

	// ����ѯʧ�ܣ��򷵻صĽ����Ϊ��
	if (mysql_real_query(&m_dbConn, sql.c_str(), (unsigned long)sql.length()) != 0 ||
		(res = mysql_store_result(&m_dbConn)) == NULL)
	{
		m_errno = mysql_errno(&m_dbConn);
		m_error = mysql_error(&m_dbConn);
		return NULL;
	}

	return new(nothrow)Result(res);
}

// ִ�и���/����/ɾ�����. ��ʧ�ܣ��򷵻�false.
bool DBConnector::Update(string sql)
{
	if (mysql_real_query(&m_dbConn, sql.c_str(), (unsigned long)sql.length()) != 0)
	{
		m_errno = mysql_errno(&m_dbConn);
		m_error = mysql_error(&m_dbConn);
		return false;
	}

	return true;
}

// ��ȡ������
unsigned int DBConnector::GetErrno()
{
	return m_errno;
}

// ��ȡ������Ϣ
const char* DBConnector::GetErrMsg()
{
	return m_error.c_str();
}


//======================================= Result ===============================================//
// ��¼������и��ֶε�˳��
Result::Result(MYSQL_RES* res)
:m_res(res)
{
	if (m_res == NULL)
		return;

	for (unsigned int i = 0; i < m_res->field_count; i++)
	{
		MYSQL_FIELD* field = m_res->fields + i;
		string fidname = field->name;
		transform(fidname.begin(), fidname.end(), fidname.begin(), ::toupper);
		m_fidmap[fidname] = i;
	}
}

// ��������
Result::~Result()
{
	mysql_free_result(m_res);
}

// �������Ϊ�գ��򷵻�true
bool Result::IsEmpty()
{
	// ������ΪResultSet����IsEmpty���ǣ�
	// ����ָ�����ÿն���ʱ��thisΪNULL
	return (this==NULL || m_res == NULL);
}

// ���ؽ������������
my_ulonglong Result::GetRowCount()
{
	return (m_res ? m_res->row_count : 0);
}

// ���α��Ƶ���һ��
bool Result::FetchOne()
{
	m_row = mysql_fetch_row(m_res);
	if (m_row == NULL)
		return false;
	return true;
}

// ��ȡ��ǰ���У����ֶζ�Ӧ��ֵ����const char*�ͷ���
const char* Result::GetStr(string fidname)
{
	if (m_row == NULL)
		return NULL;

	// תΪ��д��ĸ
	string FIDNAME = fidname;
	transform(FIDNAME.begin(), FIDNAME.end(), FIDNAME.begin(), ::toupper);

	int index = m_fidmap[FIDNAME];
	return m_row[index];
}

// ��ȡ��ǰ���У����ֶζ�Ӧ��ֵ����int�ͷ���
int Result::GetInt(string fidname)
{
	if (m_row == NULL)
		return 0;

	// תΪ��д��ĸ
	string FIDNAME = fidname;
	transform(FIDNAME.begin(), FIDNAME.end(), FIDNAME.begin(), ::toupper);

	int index = m_fidmap[FIDNAME];
	return atoi(m_row[index]);
}

// ��ȡ��ǰ���У����ֶζ�Ӧ��ֵ����double�ͷ���
double Result::GetDouble(string fidname)
{
	if (m_row == NULL)
		return 0.0;

	// תΪ��д��ĸ
	string FIDNAME = fidname;
	transform(FIDNAME.begin(), FIDNAME.end(), FIDNAME.begin(), ::toupper);

	int index = m_fidmap[FIDNAME];
	return atof(m_row[index]);
}

