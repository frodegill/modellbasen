#include "messageboard.h"

#include "../../app/defines.h"
#include "../../singleton/lock.h"
#include "../../singleton/models.h"
#include "../../utils/time.h"

#include <Wt/WStandardItem>
#include <Wt/WString>


using namespace modellbasen;

MessageBoard::MessageBoard()
: m_id(INVALID_ID),
  m_posted_time(0),
  m_user_id(INVALID_ID)
{
}

bool MessageBoard::InitializeGlobalMessageboardList()
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	g_messageboard_model.clear();
	if (3 > g_messageboard_model.columnCount())
	{
		g_messageboard_model.insertColumns(0, 3 - g_messageboard_model.columnCount());
		g_messageboard_model.setHeaderData(0, Wt::Horizontal, boost::any(Wt::WString::tr("PostedTimeHeader")));
		g_messageboard_model.setHeaderData(1, Wt::Horizontal, boost::any(Wt::WString::tr("UsernameHeader")));
		g_messageboard_model.setHeaderData(2, Wt::Horizontal, boost::any(Wt::WString::tr("MessageHeader")));
	}

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	std::string message;
	Poco::UInt64 timestamp;
	std::string username;
	Poco::Data::Statement statement(*session);
	statement << "SELECT m.message, m.posted_time, u.username FROM messageboard m, user u WHERE m.user=u.id ORDER BY m.posted_time DESC LIMIT 25", //25=MAX_MESSAGEBOARD_ROWS
		Poco::Data::Keywords::into(message),
		Poco::Data::Keywords::into(timestamp),
		Poco::Data::Keywords::into(username),
		Poco::Data::Keywords::range<Poco::Data::Limit::SizeT>(0,1);

	int row = 0;
	std::string posted_time;
	while (!statement.done() && 0<statement.execute())
	{
		Time::ToString(timestamp, posted_time);
		g_messageboard_model.setItem(row, 0, new Wt::WStandardItem(Wt::WString::fromUTF8(posted_time)));
		g_messageboard_model.setItem(row, 1, new Wt::WStandardItem(Wt::WString::fromUTF8(username)));
		g_messageboard_model.setItem(row++, 2, new Wt::WStandardItem(Wt::WString::fromUTF8(message)));
	}
		
	DB.ReleaseSession(session, PocoGlue::IGNORE);

	return true;
}

bool MessageBoard::AddMessage(Poco::Data::Session* session_in_transaction, Poco::UInt32 user_id, const std::string& message)
{
	if (!session_in_transaction || INVALID_ID==user_id || message.empty())
		return false;

	Poco::UInt64 now = Time::NowUTC();
	
	DEBUG_TRY_CATCH(*session_in_transaction << "INSERT INTO messageboard (message, posted_time, user) "\
														"VALUE (?, ?, ?)",
		Poco::Data::Keywords::useRef(message), Poco::Data::Keywords::use(now), Poco::Data::Keywords::use(user_id), Poco::Data::Keywords::now;)

	//TODO

	return true;
}
