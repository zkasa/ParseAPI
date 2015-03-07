#pragma once
#include "cpprest/json.h"
#include "cpprest/details/basic_types.h"
#include "ParseObjects.h"


namespace parse {
namespace api {

class Client;
class Error;
class User;

class Users
{
public:
	Users(parse::api::Client const& client);
	~Users();

public:
	parse::api::User createUser(utility::string_t const& userName, utility::string_t const& password
		, web::json::value const& additionalFields = web::json::value::null());
	parse::api::User createUser(utility::string_t const& userName, utility::string_t const& password
		, utility::string_t const& additionalFields);
	parse::api::User loginUser(utility::string_t const& userName, utility::string_t const& password);
	parse::api::User validateSession(parse::api::User const& user);

	std::vector<parse::api::User> getUsers();
	parse::api::User getUser(utility::string_t const& userId);
	void updateUser(parse::api::User const& user);
	// forceDelete means we should delete given user by passing master key instead of session token
	parse::api::Error deleteUser(parse::api::User& user, bool forceDelete = false);

private:
	parse::api::Client _Client;
};

class User : public Object {
public:
	User(web::json::value const& json);
	~User();

	utility::string_t getName() const;
	utility::string_t getSessionToken() const;
	void setDeleted();

	bool isLoggedIn() const;

private:
};

} }