#ifndef _DATABASE_UPDATE_H_
#define _DATABASE_UPDATE_H_

#include "../app/defines.h"
#include "poco_glue.h"


namespace modellbasen
{

#define BCRYPT_HASH_LEN (64)


struct DatabaseUpdateItem {
	uint32_t    m_db_version;
	std::string m_update_query;
};


class DatabaseUpdate
{
public:
	DatabaseUpdate();
	virtual ~DatabaseUpdate();

public:
	bool GetDatabaseVersion(Poco::Data::Session* session_in_transaction, uint32_t& db_version) const;
	bool UpdateIfNeeded(Poco::Data::Session* session_in_transaction);
};

} // namespace modellbasen

#endif // _DATABASE_UPDATE_H_
