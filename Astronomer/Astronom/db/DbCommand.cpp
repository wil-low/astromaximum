#include "DbCommand.h"
#include <sqlite3.h>

DbCommand::DbCommand(sqlite3 *db)
: db_(db)
, stmt_(NULL)
{

}

DbCommand::DbCommand(sqlite3 *db, const FXString& sql)
: db_(db)
, sql_(sql)
, stmt_(NULL)
{
    wraperr (sqlite3_prepare_v2(db_, sql_.text(), sql_.length(), &stmt_, NULL));
}

DbCommand::~DbCommand()
{
     wraperr (sqlite3_finalize(stmt_));
}

int DbCommand::wraperr (int result)
{
    if (result != SQLITE_OK) {
        FXTRACE((30, "%s: %s\n", __FUNCTION__, sqlite3_errmsg(db_)));
    }
    return result;
}
