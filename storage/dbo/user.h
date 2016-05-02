#ifndef _USER_H_
#define _USER_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>
#include <vector>


namespace modellbasen
{

class UserManager;

class User {
friend class UserManager;

public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::User>;
	bool operator <(const User& user) const {return m_id < user.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	User();

public:
	IdType       GetId() const {return m_id;}
	const std::string& GetUsername() const {return m_username;}
	const std::string& GetPasswordHash() const {return m_bcrypt_password_hash;}
	const std::string& GetEmail() const {return m_email;}
	IdType       GetProfilePictureId() const {return m_profile_picture_id;}

	void               SetUsername(const std::string& username) {m_username = username;}
	void               SetPasswordHash(const std::string& password_hash) {m_bcrypt_password_hash = password_hash;}

public:
	bool HasTag(const std::string& tag) const;

private:
	IdType m_id;
	std::string  m_username;
	std::string  m_bcrypt_password_hash;
	std::string  m_email;
	IdType m_profile_picture_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::User>
{
public:
	static std::size_t size() {return 5;}

	static void bind(std::size_t pos, const modellbasen::User& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_username, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_bcrypt_password_hash, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_email, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_profile_picture_id, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::User& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_username, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_bcrypt_password_hash, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_email, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_profile_picture_id, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::User& obj, const modellbasen::User& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_username, defVal.m_username, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_bcrypt_password_hash, defVal.m_bcrypt_password_hash, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_email, defVal.m_email, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_profile_picture_id, defVal.m_profile_picture_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _USER_H_
