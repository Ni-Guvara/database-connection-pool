#include <iostream>

#include "CommonConnectionPool.h"
#include "Connection.h"

int main()
{
   /* char sql[1024] = { 0 };
    sprintf(sql, "insert into user(name, sex, age) values('%s', '%s', '%d')", "zhangsan", "male", 21);
    
    Connection* conn = new Connection;
    conn->connect("127.0.0.1", 3306, "root", "123456", "chat");
    conn->update(sql);*/

    CommonConnectionPool::getConnectionPool().loadConfigFile();
    return 0;
    
}
