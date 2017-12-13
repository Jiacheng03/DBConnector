#pragma once

#include <mysql.h>
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
	// 成员初始化
	DBConnector(string host, string user, string passwd, string dbname, unsigned int port, string charset = "utf8");
	
	// 清理MYSQL对象
	~DBConnector();

	// 初始化MYSQL对象，连接mysql. 若失败，则返回false.
	bool Initialize();

	// 执行查询语句, 返回Result指针，建议使用ResultSet类进行处理
	// 若返回为空，则返回NULL，即结果集为空
	Result* Query(string sql);

	// 执行更新/插入/删除语句. 若失败，则返回false.
	bool Update(string sql);

private:
	string m_host;
	string m_user;
	string m_passwd;
	string m_dbname;
	unsigned int m_port;
	string m_charset;

	MYSQL m_dbConn;
};

//======================================= Result ===============================================//
class Result
{
public:
	// 记录结果集中各字段的顺序
	Result(MYSQL_RES* res);

	// 清理结果集
	~Result();

	// 若结果集为空，则返回true
	bool IsEmpty();

	// 返回结果集的总行数
	my_ulonglong GetRowCount();

	// 将游标移到下一行
	bool FetchOne();

	// 获取当前行中，该字段对应的值，以const char*型返回
	const char* GetStr(string fidname);

	// 获取当前行中，该字段对应的值，以int型返回
	int GetInt(string fidname);

	// 获取当前行中，该字段对应的值，以double型返回
	double GetDouble(string fidname);

private:
	MYSQL_RES * m_res;
	MYSQL_ROW m_row;
	unordered_map<string, int> m_fidmap;	
};
