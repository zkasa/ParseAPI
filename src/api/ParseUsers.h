#pragma once
#include "cpprest/json.h"
#include "cpprest/details/basic_types.h"
#include "ParseObjects.h"


namespace parse {
namespace api {

class Client;
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
	std::vector<parse::api::User> getUsers();
	parse::api::User getUser(utility::string_t const& userId);
	void updateUser(parse::api::User const& user);
	void deleteUser(parse::api::User& user);

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