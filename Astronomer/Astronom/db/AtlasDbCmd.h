#ifndef ATLASDBCMD_H
#define ATLASDBCMD_H

#include "DbCommand.h"


class AtlasDbCmd : public DbCommand
{
public:
    AtlasDbCmd(sqlite3 *db);
    virtual ~AtlasDbCmd();
protected:
private:
};

#endif // ATLASDBCMD_H
