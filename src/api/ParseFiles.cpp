#include <cpprest/filestream.h>
#include "constants.h"
#include "ParseClient.h"
#include "ParseObjects.h"

#include "ParseFiles.h"

namespace {
	const utility::string_t CLASS_FILE = U("File");
	const utility::string_t FIELD_FILE = U("file");
	const utility::string_t FIELD_NAME = U("name");
	const utility::string_t FIELD_URL = U("url");

	utility::string_t getFileNameFromPath(utility::string_t const& filePath) {
		auto it = std::find_if(filePath.rbegin(), filePath.rend(),
			[](utility::string_t::value_type c) { return c == U('\\') || c == U('/'); } );
		return utility::string_t(it.base(), filePath.end());
	}
}

parse::api::Files::Files(parse::api::Client const& client)
	: _Client(client)
{

}

parse::api::Files::~Files()
{

}

parse::api::File parse::api::Files::uploadFile(utility::string_t const& filePath)
{
	web::http::http_request req(web::http::methods::POST);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(FILES_URI);
	builder.append_path(getFileNameFromPath(filePath));

	auto task = Concurrency::streams::file_stream<unsigned char>::open_istream(filePath);
	task.wait();

	req.set_body(task.get());
	req.set_request_uri(builder.to_uri());

	auto json = _Client.requestJsonSync(req);

	if (json.has_field(FIELD_NAME) && json.has_field(FIELD_URL)) {
		// when we upload file to Parse we MUST create an object
		// and associate them with created file to be able control the file 
		json[FIELD_TYPE] = web::json::value::string(TYPE_FILE);
		Objects objs(_Client, CLASS_FILE);
		Object obj = objs.createObject(web::json::value::null());
		obj.associateFile(FIELD_FILE, json);
		objs.updateObject(obj);
		return File(obj);
	}
	return File(Object::null());
}

parse::api::File parse::api::Files::uploadFile(utility::string_t const& filePath, parse::api::Object& parent
	, utility::string_t const& fieldName)
{
	File file = uploadFile(filePath);
	file.setPointer(fieldName, parent);
	return file;
}

std::vector<parse::api::File> parse::api::Files::getFiles(parse::api::Object const& parent /*= parse::api::Object::null()*/)
{
	std::vector<parse::api::File> result;
	if (parent == Object::null()) {
		// get all files of application
		for (auto obj : Objects(_Client, CLASS_FILE).getObjects()) {
			result.push_back(File(obj));
		}
	}
	else {
		// get all files with specified parent
		// TODO
	}
	return result;
}

bool parse::api::Files::deleteFile(parse::api::File& file)
{
	web::http::http_request req(web::http::methods::DEL);
	_Client.fillCommonParseHeaders(req, true);

	web::http::uri_builder builder(FILES_URI);
	builder.append_path(file.getFileName());

	req.set_request_uri(builder.to_uri());

	// when deleting files Parse doesn't return valid json!
	// so we can check only http status code
	auto resp = _Client.requestSync(req);
	if (resp.status_code() == 200) {
		Objects(_Client, CLASS_FILE).deleteObject(file);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

parse::api::File::File(parse::api::Object const& obj)
	: Object(obj.getClassName(), obj.getJson())
{

}

parse::api::File::~File()
{

}

utility::string_t parse::api::File::getFileName() const {
	return getJson().at(FIELD_FILE).at(FIELD_NAME).as_string();
}

utility::string_t parse::api::File::getFileUri() const {
	return getJson().at(FIELD_FILE).at(FIELD_URL).as_string();
}
