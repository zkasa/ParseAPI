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
	File uploadFile(utility::string_t const& filePath, parse::api::Object& parent = parse::api::Object::null()
		, utility::string_t const& fieldName = U(""));
	File uploadFile(std::vector<unsigned char> const& data, utility::string_t const& fileName
		, parse::api::Object& parent = parse::api::Object::null(), utility::string_t const& fieldName = U(""));
	// get list of files by custom query. about queries read official Parse documentation
	// https://www.parse.com/docs/rest#queries
	// to get all files of the application just leave "query" parameter empty
	std::vector<parse::api::File> getFiles(utility::string_t const& query = U(""));
	// get all files belongs to Object passed as parent
	std::vector<parse::api::File> getFilesOf(parse::api::Object const& parent
		, utility::string_t const& fieldName);
	void updateFile(parse::api::File& file);
	bool deleteFile(parse::api::File& file);

private:
	File uploadRequest(web::http::http_request& req, parse::api::Object& parent, utility::string_t const& fieldName);

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