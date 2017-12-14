#include <iostream>
#include "DBConnector.h"

using namespace std;

int main()
{
	const char host[] = "localhost";
	const char user[] = "jc";	
	const char passwd[] = "asdqwe123";
	const char dbname[] = "test";
	unsigned int port = 3306;

	// 初始化连接
	DBConnector dbconn(host, user, passwd, dbname, port, "gbk");
	if (!dbconn.Initialize())
	{
		cerr << "DBConnector Connect Fail ..." << endl;
		system("pause");
		exit(1);
	}

	// 查询，并获取结果集
	string sql = "SELECT NAME,SIZE FROM BOOK";
	ResultSet res(dbconn.Query(sql));

	if (!res)
	{
		cout << "result is empty" << endl;
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