#ifndef DBCOMMAND_H
#define DBCOMMAND_H

#include <fx.h>

struct sqlite3;
struct sqlite3_stmt;

class DbCommand
{
public:
    DbCommand(sqlite3 *db);
    DbCommand(sqlite3 *db, const FXString& sql);
    virtual ~DbCommand();
protected:
    int wraperr (int result);
private:
    sqlite3 *db_;
    FXString sql_;
    sqlite3_stmt *stmt_;
};

#endif // DBCOMMAND_H
