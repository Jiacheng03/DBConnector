#include <iostream>
#include "DBConnector.h"

using namespace std;

int main()
{
	const char host[] = "192.168.245.128";
	const char user[] = "jc";	
	const char passwd[] = "asdqwe";
	const char dbname[] = "test";
	unsigned int port = 3306;

	// 初始化连接
	DBConnector dbconn(host, user, passwd, dbname, port, "gbk");
	if (!dbconn.Initialize())
	{		
		cerr << dbconn.GetErrno() << endl;
		cerr << dbconn.GetErrMsg() << endl;
		system("pause");
		exit(1);
	}

	// 查询，并获取结果集
	string sql = "SELECT NAME,SIZE FROM BOOK";
	ResultSet res(dbconn.Query(sql));

	if (!res)
	{
		cerr << dbconn.GetErrMsg() << endl;
		system("pause");
		exit(2);
	}

	// 从结果集中逐行取数据
	while (res->FetchOne())
	{
		cout << "BOOKNAME: " << res->GetStr("NAME") << endl;
		cout << "    SIZE: " << res->GetInt("SIZE") << endl;
	}

	system("pause");
	return 0;
}