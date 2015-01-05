#include "ParseClient.h"

#include "ParseUsers.h"

const utility::string_t USERS_URI = U("/1/users");

parse::api::Users::Users(parse::api::Client const& client)
	: _Client(client)
{

}

parse::api::Users::~Users()
{

}

parse::api::User parse::api::Users::createUser(utility::string_t const& userName, utility::string_t const& password 
	, web::json::value const& additionalFields /* = web::json::value::null() */)
{
	web::http::http_request req(web::http::methods::POST);
	_Client.fillCommonParseHeaders(req);

	web::json::value objUser;
	objUser[L"username"] = web::json::value::string(userName);
	objUser[L"password"] = web::json::value::string(password);
	
	if (!additionalFields.is_null() && additionalFields.is_object()) {
		for (auto field : additionalFields.as_object()) {
			objUser[field.first] = field.second;
		}
	}
	req.set_body(objUser);
	req.set_request_uri(USERS_URI);

	return User(_Client.requestSync(req));
}

parse::api::User parse::api::Users::createUser(utility::string_t const& userName, utility::string_t const& password
	, utility::string_t const& additionalFields)
{
	auto val = web::json::value::parse(additionalFields);
	return createUser(userName, password, val);
}

std::vector<parse::api::User> parse::api::Users::getUsers()
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);
	req.set_request_uri(USERS_URI);

	auto json = _Client.requestSync(req);
	std::vector<parse::api::User> users;
	if (json.has_field(U("results")) && json[U("results")].is_array()) {
		for (auto u : json[U("results")].as_array()) {
			users.push_back(User(u));
		}
	}
	return users;
}

parse::api::User parse::api::Users::getUser(utility::string_t const& userId)
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(USERS_URI);
	builder.append_path(userId);

	req.set_request_uri(builder.to_uri());

	return User(_Client.requestSync(req));
}

void parse::api::Users::updateUser(parse::api::User const& user)
{
	web::http::http_request req(web::http::methods::PUT);
	_Client.fillCommonParseHeaders(req);
	req.headers().add(U("X-Parse-Session-Token"), user.getSessionToken());

	web::http::uri_builder builder(USERS_URI);
	builder.append_path(user.getId());

	// TODO: update only modified fields!
	req.set_body(user.getJson());
	req.set_request_uri(builder.to_uri());

	_Client.requestSync(req);
	//TODO: check result
}

void parse::api::Users::deleteUser(parse::api::User& user)
{
	web::http::http_request req(web::http::methods::DEL);
	_Client.fillCommonParseHeaders(req);
	req.headers().add(U("X-Parse-Session-Token"), user.getSessionToken());

	web::http::uri_builder builder(USERS_URI);
	builder.append_path(user.getId());

	req.set_request_uri(builder.to_uri());

	_Client.requestSync(req);
	//TODO: check result

	user.setDeleted();
}

//////////////////////////////////////////////////////////////////////////

parse::api::User::User(web::json::value const& json)
	: Object(U("_User"), json)
{

}

parse::api::User::~User()
{

}

utility::string_t parse::api::User::getName() const {
	return getField(U("username"));
}

bool parse::api::User::isLoggedIn() const {
	return !getField(U("sessionToken")).empty();
}

utility::string_t parse::api::User::getSessionToken() const {
	return getField(U("sessionToken"));
}

void parse::api::User::setDeleted() {
	resetJson();
}

