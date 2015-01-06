#pragma once
#include "cpprest/details/basic_types.h"

namespace parse {
namespace api {

class Client;
class Object;
class File;

class Files
{
public:
	Files(parse::api::Client const& client);
	~Files();

public:
	File uploadFile(utility::string_t const& filePath);
	File uploadFile(utility::string_t const& filePath, parse::api::Object& parent
		, utility::string_t const& fieldName);
	std::vector<parse::api::File> getFiles(parse::api::Object const& parent = parse::api::Object::null());
	bool deleteFile(parse::api::File& file);

private:
	parse::api::Client _Client;
};

class File : public Object
{
public:
	File(parse::api::Object const& obj);
	~File();

public:
	utility::string_t getFileName() const;
	utility::string_t getFileUri() const;
};

}
}