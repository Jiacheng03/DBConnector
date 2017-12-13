#include <iostream>
#include "DBConnector.h"

using namespace std;

int main()
{
	const char host[] = "localhost";
	const char user[] = "root";	
	const char passwd[] = "asdqwe123";
	const char dbname[] = "test";
	unsigned int port = 3306;

	// ��ʼ������
	DBConnector dbconn(host, user, passwd, dbname, port, "gbk");
	if (!dbconn.Initialize())
	{
		cerr << "DBConnector Connect Fail ..." << endl;
	}

	// ��ѯ������ȡ�����
	string sql = "SELECT NAME,SIZE FROM BOOK";
	ResultSet res(dbconn.Query(sql));

	// �ӽ����������ȡ����
	while (res->FetchOne())
	{
		cout << "BOOKNAME: " << res->GetStr("NAME") << endl;
		cout << "    SIZE: " << res->GetInt("SIZE") << endl;
	}

	system("pause");
	return 0;
}