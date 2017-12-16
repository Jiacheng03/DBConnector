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

	// ��ʼ������
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

	// ��ѯ������ȡ�����
	string sql = "SELECT NAME,SIZE FROM BOOK";
	ResultSet res(dbconn.Query(sql));

	if (!res)
	{
		cerr << dbconn.GetErrMsg() << endl;
		system("pause");
		exit(2);
	}

	// �ӽ����������ȡ����
	while (res->FetchOne())
	{
		cout << "BOOKNAME: " << res->GetStr("NAME") << endl;
		cout << "    SIZE: " << res->GetInt("SIZE") << endl;
	}

#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}