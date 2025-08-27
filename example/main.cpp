#include <iostream>

#include "CommonConnectionPool.h"
#include "Connection.h"

int main()
{
    clock_t start = clock();
    // for (int i = 0; i < 10000; i++) // 1000-1489 ms  5000-7812 ms 10000-24766 ms
    // {
    //     Connection conn;
    //     char sql[1024] = {0};
    //     sprintf(sql, "insert into person(name, sex, age) values('%s', '%s', '%d')", "zhangsan", "M", 21);
    //     if (conn.connect("127.0.0.1", 3306, "root", "123456", "chat"))
    //     {
    //         cout << "连接成功" << endl;
    //         conn.update(sql);
    //     }
    //     else
    //     {
    //         cout << "连接失败" << endl;
    //     }
    // }
    CommonConnectionPool *pool = CommonConnectionPool::getConnectionPool(); // 1000-1605 ms 5000-8361 ms 10000-16460 ms
    for (int i = 0; i < 1000; i++)
    {
        shared_ptr<Connection> conn = pool->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into person(name, sex, age) values('%s', '%s', '%d')", "zhangsan", "M", 21);
        conn->update(sql);
    }

    clock_t end = clock();

    cout << end - start << " " << "ms" << endl;

    return 0;
}
