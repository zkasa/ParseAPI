#include "constants.h"
#include "ParseClient.h"

#include "ParseObjects.h"

parse::api::Objects::Objects(parse::api::Client const& client, utility::string_t const& className)
	: _Client(client)
	, _ClassName(className)
{

}

parse::api::Objects::~Objects()
{

}

parse::api::Object parse::api::Objects::createObject(web::json::value const& json /* = web::json::value::null() */)
{
	web::http::http_request req(web::http::methods::POST);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(CLASSES_URI);
	builder.append_path(_ClassName);

	req.set_body(json);
	req.set_request_uri(builder.to_uri());

	return Object(_ClassName, _Client.requestJsonSync(req));
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

	auto json = _Client.requestJsonSync(req);
	std::vector<parse::api::Object> objects;
	if (json.has_field(FIELD_RESULTS) && json[FIELD_RESULTS].is_array()) {
		for (auto o : json[FIELD_RESULTS].as_array()) {
			objects.push_back(Object(_ClassName, o));
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

	return Object(_ClassName, _Client.requestJsonSync(req));
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

	auto json = _Client.requestJsonSync(req);
	//TODO: check result
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

	auto json = _Client.requestJsonSync(req);
	//TODO: check result
}

//////////////////////////////////////////////////////////////////////////

parse::api::Object::Object(utility::string_t const& className, web::json::value const& json)
	: _ClassName(className)
	, _Json(json)
{

}

parse::api::Object::~Object()
{
	_Json = web::json::value::null();
}

parse::api::Object parse::api::Object::null() {
	return Object(U(""), web::json::value::null());
}

utility::string_t const& parse::api::Object::getClassName() const {
	return _ClassName;
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

void parse::api::Object::setPointer(utility::string_t const& fieldName, parse::api::Object const& obj)
{
	auto pointer = web::json::value::object();
	pointer[FIELD_TYPE] = web::json::value::string(TYPE_POINTER);
	pointer[FIELD_CLASS_NAME] = web::json::value::string(obj.getClassName());
	pointer[FIELD_OBJECT_ID] = web::json::value::string(obj.getId());

	_Json[fieldName] = pointer;
}

void parse::api::Object::associateFile(utility::string_t const& fieldName, web::json::value const& json) {
	_Json[fieldName] = json;
}

utility::string_t parse::api::Object::getId() const {
	return getField(FIELD_OBJECT_ID);
}

utility::string_t parse::api::Object::getCreatedAt() const {
	return getField(FIELD_CREATED_AT);
}

void parse::api::Object::resetJson() {
	_Json = web::json::value::null();
}

bool parse::api::Object::isValid() const {
	return _Json != web::json::value::null() 
		&& !getId().empty();
}

bool parse::api::Object::operator==(parse::api::Object const& obj) const {
	return getClassName() == obj.getClassName()
		&& getJson() == obj.getJson();
}

bool parse::api::Object::operator!=(parse::api::Object const& obj) const {
	return !(*this == obj);
}

utility::string_t parse::api::Object::operator[](utility::string_t const& fieldName) const {
	return getField(fieldName);
}
