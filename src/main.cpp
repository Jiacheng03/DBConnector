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
#ifdef _MSC_VER
	DBConnector dbconn(host, user, passwd, dbname, port, "gbk");
#else
	DBConnector dbconn(host, user, passwd, dbname, port);
#endif

	if (!dbconn.Initialize())
	{		
		cerr << dbconn.GetErrno() << endl;
		cerr << dbconn.GetErrMsg() << endl;
		system("pause");
		exit(1);
	}

	// 查询，并获取结果集
	string sql = "SELECT * FROM BOOK";
	ResultSet res(dbconn.Query(sql));

	if (res->IsEmpty())
	{
		cerr << dbconn.GetErrMsg() << endl;
		system("pause");
		exit(2);
	}

	// 从结果集中逐行取数据
	while (res->FetchOne())
	{
		cout << "      ID: " << res->GetInt("id") << endl;
		cout << "BOOKNAME: " << res->GetStr("NAME") << endl;
		cout << "    SIZE: " << res->GetInt("SIZE") << endl;
		cout << "    DATE: " << res->GetStr("DATE") << endl;
	}

#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}