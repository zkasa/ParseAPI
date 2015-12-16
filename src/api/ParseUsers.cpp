#include "constants.h"
#include "ParseClient.h"
#include "ParseError.h"

#include "ParseUsers.h"

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
	objUser[FIELD_USERNAME] = web::json::value::string(userName);
	objUser[FIELD_PASSWORD] = web::json::value::string(password);
	
	if (!additionalFields.is_null() && additionalFields.is_object()) {
		for (auto field : additionalFields.as_object()) {
			objUser[field.first] = field.second;
		}
	}
	req.set_body(objUser);
	req.set_request_uri(USERS_URI);

	return User(_Client.requestJsonSync(req));
}

parse::api::User parse::api::Users::createUser(utility::string_t const& userName, utility::string_t const& password
	, utility::string_t const& additionalFields)
{
	auto val = web::json::value::parse(additionalFields);
	return createUser(userName, password, val);
}

parse::api::User parse::api::Users::loginUser(utility::string_t const& userName, utility::string_t const& password)
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(LOGIN_URI);

	builder.append_query(U("username"), userName, true);
	builder.append_query(U("password"), password, true);

	req.set_request_uri(builder.to_uri());

	return User(_Client.requestJsonSync(req));
}

parse::api::User parse::api::Users::validateSession(parse::api::User const& user)
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);
	req.headers().add(HEADER_SESSION_TOKEN, user.getSessionToken());

	req.set_request_uri(USERS_ME_URI);

	return User(_Client.requestJsonSync(req));
}

std::vector<parse::api::User> parse::api::Users::getUsers()
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);

	//req.set_request_uri(USERS_URI);
	web::http::uri_builder builder(USERS_URI);
	//builder.append_path(userId);
	builder.append_query(U("limit=200"));
	
	req.set_request_uri(builder.to_uri());

	auto json = _Client.requestJsonSync(req);
	std::vector<parse::api::User> users;
	if (json.has_field(FIELD_RESULTS) && json[FIELD_RESULTS].is_array()) {
		for (auto u : json[FIELD_RESULTS].as_array()) {
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

	return User(_Client.requestJsonSync(req));
}

void parse::api::Users::updateUser(parse::api::User const& user)
{
	web::http::http_request req(web::http::methods::PUT);
	_Client.fillCommonParseHeaders(req);
	req.headers().add(HEADER_SESSION_TOKEN, user.getSessionToken());

	web::http::uri_builder builder(USERS_URI);
	builder.append_path(user.getId());

	// TODO: update only modified fields!
	req.set_body(user.getJson());
	req.set_request_uri(builder.to_uri());

	_Client.requestJsonSync(req);
	//TODO: check result
}

parse::api::Error parse::api::Users::deleteUser(parse::api::User& user, bool forceDelete /* = false */)
{
	web::http::http_request req(web::http::methods::DEL);

	// в обычных условиях мы можем изменять поля или удалять юзера
	// только имея валидный session key
	// однако, если мы получаем юзера через getUser/getUsers мы НЕ имеем 
	// в полученных параметрах session key, он передается только при логине
	// если в подобных случаях требуется удалить пользователя, следует передать
	// forceDelete = true
	if (forceDelete && user.getSessionToken().empty()) {
		_Client.fillCommonParseHeaders(req, true);
	}
	else {
		_Client.fillCommonParseHeaders(req);
		req.headers().add(HEADER_SESSION_TOKEN, user.getSessionToken());
	}

	web::http::uri_builder builder(USERS_URI);
	builder.append_path(user.getId());

	req.set_request_uri(builder.to_uri());

	Error res = _Client.requestJsonSync(req);

	if (!res.isError())
		user.setDeleted();

	return res;
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
	return getField(FIELD_USERNAME);
}

bool parse::api::User::isLoggedIn() const {
	return !getField(FIELD_SESSION_TOKEN).empty();
}

utility::string_t parse::api::User::getSessionToken() const {
	return getField(FIELD_SESSION_TOKEN);
}

void parse::api::User::setDeleted() {
	resetJson();
}

