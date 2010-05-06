#include "DbCommand.h"
#include <sqlite3.h>

DbCommand::DbCommand(sqlite3 *db, const FXString& sql)
: db_(db)
, sql_(sql)
{

}

DbCommand::~DbCommand()
{
    //dtor
}

int DbCommand::wraperr (int result)
{
    if (result != SQLITE_OK) {
        FXTRACE((30, "%s: %s\n", __FUNCTION__, sqlite3_errmsg(db_)));
    }
    return result;
}
