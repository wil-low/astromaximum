#include "AtlasDbCmd.h"

AtlasDbCmd::AtlasDbCmd(sqlite3 *db)
: DbCommand(db)
{
    //ctor
}

AtlasDbCmd::~AtlasDbCmd()
{
    //dtor
}
