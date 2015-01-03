#include "ParseClient.h"

#include "ParseObjects.h"

const utility::string_t CLASSES_URI = U("/1/classes");

parse::api::Objects::Objects(parse::api::Client const& client, utility::string_t const& className)
	: _Client(client)
	, _ClassName(className)
{

}

parse::api::Objects::~Objects()
{

}

parse::api::Object parse::api::Objects::createObject(web::json::value const& json)
{
	web::http::http_request req(web::http::methods::POST);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(CLASSES_URI);
	builder.append_path(_ClassName);

	req.set_body(json);
	req.set_request_uri(builder.to_uri());

	return Object(_Client.requestSync(req));
}

parse::api::Object parse::api::Objects::createObject(utility::string_t const& jsonStr)
{
	auto json = web::json::value::parse(jsonStr);
	return createObject(json);
}

std::vector<parse::api::Object> parse::api::Objects::getObjects(utility::string_t const& query /* = U("") */)
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(CLASSES_URI);
	builder.append_path(_ClassName);

	if (!query.empty()) {
		builder.append_query(query, true);
	}
	req.set_request_uri(builder.to_uri());

	auto json = _Client.requestSync(req);
	std::vector<parse::api::Object> objects;
	if (json.has_field(U("results")) && json[U("results")].is_array()) {
		for (auto o : json[U("results")].as_array()) {
			objects.push_back(Object(o));
		}
	}
	return objects;
}

parse::api::Object parse::api::Objects::getObject(utility::string_t const& objectId)
{
	web::http::http_request req(web::http::methods::GET);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(CLASSES_URI);
	builder.append_path(_ClassName);
	builder.append_path(objectId);

	req.set_request_uri(builder.to_uri());

	return Object(_Client.requestSync(req));
}

void parse::api::Objects::updateObject(parse::api::Object& object)
{
	web::http::http_request req(web::http::methods::PUT);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(CLASSES_URI);
	builder.append_path(_ClassName);
	builder.append_path(object.getId());

	// TODO: update only modified fields!
	req.set_body(object.getJson());
	req.set_request_uri(builder.to_uri());

	//TODO: check result
	object = Object(_Client.requestSync(req));
}

void parse::api::Objects::deleteObject(parse::api::Object& object)
{
	deleteObject(object.getId());
	object.resetJson();
}

void parse::api::Objects::deleteObject(utility::string_t const& objectId)
{
	web::http::http_request req(web::http::methods::DEL);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(CLASSES_URI);
	builder.append_path(_ClassName);
	builder.append_path(objectId);

	req.set_request_uri(builder.to_uri());

	_Client.requestSync(req);
	//TODO: check result
}

//////////////////////////////////////////////////////////////////////////

parse::api::Object::Object(web::json::value const& json)
	: _Json(json)
{

}

parse::api::Object::~Object()
{
	_Json = web::json::value::null();
}

web::json::value const& parse::api::Object::getJson() const {
	return _Json;
}

utility::string_t parse::api::Object::getField(utility::string_t const& fieldName) const
{
	if (!_Json.has_field(fieldName))
		return utility::string_t();
	return _Json.at(fieldName).as_string();
}

void parse::api::Object::setField(utility::string_t const& fieldName, utility::string_t const& value) {
	_Json[fieldName] = web::json::value::string(value);
}

void parse::api::Object::setField(utility::string_t const& fieldName, double const& value) {
	_Json[fieldName] = web::json::value::number(value);
}

void parse::api::Object::setField(utility::string_t const& fieldName, int32_t const& value) {
	_Json[fieldName] = web::json::value::number(value);
}

void parse::api::Object::setField(utility::string_t const& fieldName, bool const& value) {
	_Json[fieldName] = web::json::value::boolean(value);
}

utility::string_t parse::api::Object::getId() const {
	return getField(U("objectId"));
}

utility::string_t parse::api::Object::getCreatedAt() const {
	return getField(U("createdAt"));
}

void parse::api::Object::resetJson() {
	_Json = web::json::value::null();
}

bool parse::api::Object::isValid() const {
	return _Json != web::json::value::null() 
		&& !getId().empty()
		&& !getCreatedAt().empty();
}
