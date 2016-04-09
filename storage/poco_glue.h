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
#include <Poco/Nullable.h>
#include <Poco/Data/SessionPool.h>

#include "../singleton/logging.h"
#ifdef DEBUG
# define DEBUG_TRY_CATCH(...)	try {__VA_ARGS__} \
                              catch (Poco::Exception e) {::Log("error")<<e.displayText();} \
                              catch (...) {::Log("error")<<"Got generic exception";}
#else
# define DEBUG_TRY_CATCH(...)	__VA_ARGS__
#endif

#define IF_NO_ROWS(stmt,session,...) Poco::Data::Statement stmt(session);\
                                     stmt << __VA_ARGS__; \
                                     if (0==stmt.execute())

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
