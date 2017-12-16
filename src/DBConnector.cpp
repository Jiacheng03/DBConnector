#include "DBConnector.h"
#include <algorithm>

//================================= DBConnector ======================================//
// 成员初始化
DBConnector::DBConnector(string host, string user, string passwd, string dbname, unsigned int port, string charset)
:m_host(host), m_user(user), m_passwd(passwd), m_dbname(dbname), m_port(port), m_charset(charset)
{}

// 清理MYSQL对象
DBConnector::~DBConnector()
{
	mysql_close(&m_dbConn);
}

// 初始化MYSQL对象，连接mysql. 
// 若失败，则返回false.
bool DBConnector::Initialize()
{
	mysql_init(&m_dbConn);

	// 连接mysql
	MYSQL * conn = mysql_real_connect(&m_dbConn, m_host.c_str(), m_user.c_str(),
		m_passwd.c_str(), m_dbname.c_str(), m_port, NULL, 0);

	// 若连接成功，设置连接使用的字符集
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

// 执行查询语句, 返回Result指针，建议使用ResultSet类进行处理
// 若返回为空，则返回NULL，即结果集为空
Result* DBConnector::Query(string sql)
{
	MYSQL_RES * res = NULL;

	// 若查询失败，或返回的结果集为空
	if (mysql_real_query(&m_dbConn, sql.c_str(), (unsigned long)sql.length()) != 0 ||
		(res = mysql_store_result(&m_dbConn)) == NULL)
	{
		m_errno = mysql_errno(&m_dbConn);
		m_error = mysql_error(&m_dbConn);
		return NULL;
	}

	return new(nothrow)Result(res);
}

// 执行更新/插入/删除语句. 若失败，则返回false.
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

// 获取错误码
unsigned int DBConnector::GetErrno()
{
	return m_errno;
}

// 获取错误信息
const char* DBConnector::GetErrMsg()
{
	return m_error.c_str();
}


//======================================= Result ===============================================//
// 记录结果集中各字段的顺序
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

// 清理结果集
Result::~Result()
{
	mysql_free_result(m_res);
}

// 若结果集为空，则返回true
bool Result::IsEmpty()
{
	// 这里是为ResultSet调用IsEmpty考虑：
	// 智能指针引用空对象时，this为NULL
	return (this==NULL || m_res == NULL);
}

// 返回结果集的总行数
my_ulonglong Result::GetRowCount()
{
	return (m_res ? m_res->row_count : 0);
}

// 将游标移到下一行
bool Result::FetchOne()
{
	m_row = mysql_fetch_row(m_res);
	if (m_row == NULL)
		return false;
	return true;
}

// 获取当前行中，该字段对应的值，以const char*型返回
const char* Result::GetStr(string fidname)
{
	if (m_row == NULL)
		return NULL;

	// 转为大写字母
	string FIDNAME = fidname;
	transform(FIDNAME.begin(), FIDNAME.end(), FIDNAME.begin(), ::toupper);

	int index = m_fidmap[FIDNAME];
	return m_row[index];
}

// 获取当前行中，该字段对应的值，以int型返回
int Result::GetInt(string fidname)
{
	if (m_row == NULL)
		return 0;

	// 转为大写字母
	string FIDNAME = fidname;
	transform(FIDNAME.begin(), FIDNAME.end(), FIDNAME.begin(), ::toupper);

	int index = m_fidmap[FIDNAME];
	return atoi(m_row[index]);
}

// 获取当前行中，该字段对应的值，以double型返回
double Result::GetDouble(string fidname)
{
	if (m_row == NULL)
		return 0.0;

	// 转为大写字母
	string FIDNAME = fidname;
	transform(FIDNAME.begin(), FIDNAME.end(), FIDNAME.begin(), ::toupper);

	int index = m_fidmap[FIDNAME];
	return atof(m_row[index]);
}

