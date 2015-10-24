#ifndef _POCO_GLUE_H_
#define _POCO_GLUE_H_

#ifdef USE_ODBC_CONNECTION
# include <Poco/Data/ODBC/Connector.h>
# define DB_CONNECTION_TYPE "ODBC"
# define DB_CONNECTION_STRING_KEY "ODBCConnectionString"
#else
# include <Poco/Data/MySQL/Connector.h>
# define DB_CONNECTION_TYPE "MySQL"
# define DB_CONNECTION_STRING_KEY "MySQLConnectionString"
#endif

#include <Poco/Bugcheck.h>
#include <Poco/Data/Common.h>
#include <Poco/Data/SessionPool.h>


namespace modellbasen
{

class PocoGlue
{
public:
	enum TransactionCommand {
		COMMIT,
		ROLLBACK,
		IGNORE
	};

public:
	PocoGlue();
	virtual ~PocoGlue();

	bool Initialize(const std::string& connection_string);

public:
	bool CreateSession(Poco::Data::Session*& session); //Call ReleaseSession when done
	void ReleaseSession(Poco::Data::Session* session, TransactionCommand command);

public:
	static bool CommitTransaction(Poco::Data::Session* session);
	static bool RollbackTransaction(Poco::Data::Session* session);

private:
	bool UpdateDatabaseIfNeeded();

private:
	Poco::Data::SessionPool* m_pool;
};

} // namespace modellbasen

#endif // _POCO_GLUE_H_
