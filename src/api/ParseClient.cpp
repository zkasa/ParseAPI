#include "constants.h"
#include "ParseClient.h"

parse::api::Client::Client(utility::string_t const& appKey, utility::string_t const& restKey 
	, utility::string_t const& masterKey)
	: _MainUri(MAIN_URI)
	, _AppKey(appKey)
	, _RestKey(restKey)
	, _MasterKey(masterKey)
	, _Http(getMainUri())
{

}

parse::api::Client::~Client()
{

}

utility::string_t const& parse::api::Client::getAppKey() const {
	return _AppKey;
}

utility::string_t const& parse::api::Client::getRestKey() const {
	return _RestKey;
}

utility::string_t const& parse::api::Client::getMasterKey() const {
	return _MasterKey;
}

utility::string_t const& parse::api::Client::getMainUri() const {
	return _MainUri;
}

void parse::api::Client::setMainUri(utility::string_t const& uri) {
	_MainUri = uri;
}

void parse::api::Client::fillCommonParseHeaders(web::http::http_request& req, bool addMasterKey /* = false */)
{
	req.headers().add(HEADER_APPLICATION_ID, _AppKey);
	addMasterKey ? req.headers().add(HEADER_MASTER_KEY, _MasterKey) 
		: req.headers().add(HEADER_REST_API_KEY, _RestKey);
}

web::http::http_response parse::api::Client::requestSync(web::http::http_request const& req)
{
	auto task = _Http.request(req);
	task.wait();
	return task.get();
}

web::json::value parse::api::Client::requestJsonSync(web::http::http_request const& req)
{
	auto task = _Http.request(req)
		.then([](web::http::http_response response)
	{
		return response.extract_json();
	});

	task.wait();
	return task.get();
}
