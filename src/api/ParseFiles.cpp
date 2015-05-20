#include <boost/format.hpp>
#include <cpprest/filestream.h>
#include <cpprest/streams.h>

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

parse::api::File parse::api::Files::uploadRequest(web::http::http_request& req, parse::api::Object& parent
	, utility::string_t const& fieldName)
{
	auto json = _Client.requestJsonSync(req);

	if (json.has_field(FIELD_NAME) && json.has_field(FIELD_URL)) {
		// when we upload file to Parse we MUST create an object
		// and associate them with created file to be able control the file 
		json[FIELD_TYPE] = web::json::value::string(TYPE_FILE);
		Objects objs(_Client, CLASS_FILE);
		Object obj = objs.createObject(web::json::value::null());
		obj.associateFile(FIELD_FILE, json);

		if (parent != Object::null() && !fieldName.empty()) {
			obj.setPointer(fieldName, parent);
		}

		objs.updateObject(obj);
		return File(obj);
	}
	return File(Object::null());
}

parse::api::File parse::api::Files::uploadFile(utility::string_t const& filePath
	, parse::api::Object& parent /*= parse::api::Object::null()*/, utility::string_t const& fieldName /*= U("")*/)
{
	web::http::http_request req(web::http::methods::POST);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(FILES_URI);
	builder.append_path(getFileNameFromPath(filePath));

	auto task = Concurrency::streams::file_stream<unsigned char>::open_istream(filePath);
	task.wait();

	req.set_body(task.get());
	req.set_request_uri(builder.to_uri());

	return uploadRequest(req, parent, fieldName);
}

parse::api::File parse::api::Files::uploadFile(std::vector<unsigned char> const& data, utility::string_t const& fileName, parse::api::Object& parent /*= parse::api::Object::null()*/, utility::string_t const& fieldName /*= U("")*/)
{
	web::http::http_request req(web::http::methods::POST);
	_Client.fillCommonParseHeaders(req);

	web::http::uri_builder builder(FILES_URI);
	builder.append_path(fileName);

	req.set_body(data);
	req.set_request_uri(builder.to_uri());

	return uploadRequest(req, parent, fieldName);
}

std::vector<parse::api::File> parse::api::Files::getFiles(utility::string_t const& query /* = U("") */)
{
	std::vector<parse::api::File> result;
	for (auto obj : Objects(_Client, CLASS_FILE).getObjects(query)) {
		result.push_back(File(obj));
	}
	return result;
}

std::vector<parse::api::File> parse::api::Files::getFilesOf(parse::api::Object const& parent 
	, utility::string_t const& fieldName)
{
	utility::string_t query = (boost::basic_format<utility::char_t>(
		U(R"(where={"%1%":{"__type":"Pointer","className":"%2%","objectId":"%3%"}})")
		) % fieldName % parent.getClassName() % parent.getId()).str();
	return getFiles(query);
}

bool parse::api::Files::downloadFile(parse::api::File const& file, utility::string_t const& localFilePath)
{
	using namespace concurrency::streams;
	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(localFilePath).
	then([=](ostream outFile)
	{
		*fileStream = outFile;

		// Create http_client to send the request.
		web::http::client::http_client client(file.getFileUri());

		return client.request(web::http::methods::GET);
	})
		// Handle response headers arriving.
	.then([=](web::http::http_response response)
	{
		// Write response body into the file.
		return response.body().read_to_end(fileStream->streambuf());
	})
		// Close the file stream.
	.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try {
		requestTask.wait();
	}
	catch (const std::exception &e) {
		return false;
	}
	return true;
}

void parse::api::Files::updateFile(parse::api::File& file)
{
	Object obj(file.getClassName(), file.getJson());
	Objects(_Client, file.getClassName()).updateObject(obj);
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

parse::api::File::File(web::json::value const& json)
	: Object(CLASS_FILE, json)
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
