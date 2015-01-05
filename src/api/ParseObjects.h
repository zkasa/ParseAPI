#pragma once

#include "cpprest/json.h"

namespace parse {
namespace api {

class Client;
class Object;

class Objects {
public:
	Objects(parse::api::Client const& client, utility::string_t const& className);
	~Objects();

public:
	parse::api::Object createObject(web::json::value const& json);
	parse::api::Object createObject(utility::string_t const& jsonStr);
	std::vector<parse::api::Object> getObjects(utility::string_t const& query = U(""));
	parse::api::Object getObject(utility::string_t const& objectId);
	void updateObject(parse::api::Object& object);
	void deleteObject(parse::api::Object& object);
	void deleteObject(utility::string_t const& objectId);

private:
	parse::api::Client _Client;
	utility::string_t _ClassName;
};

class Object
{
public:
	Object(utility::string_t const& className, web::json::value const& json);
	~Object();

public:
	utility::string_t const& getClassName() const;
	web::json::value const& getJson() const;
	
	utility::string_t getField(utility::string_t const& fieldName) const;
	void setField(utility::string_t const& fieldName, utility::string_t const& value);
	void setField(utility::string_t const& fieldName, double const& value);
	void setField(utility::string_t const& fieldName, int32_t const& value);
	void setField(utility::string_t const& fieldName, bool const& value);
	void setField(utility::string_t const& fieldName, parse::api::Object const& value);

	utility::string_t getId() const;
	utility::string_t getCreatedAt() const;

	bool isValid() const;
	void resetJson();

private:
	utility::string_t _ClassName;
	web::json::value _Json;
};

}
}