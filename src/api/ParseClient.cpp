#include "ParseClient.h"

parse::api::Client::Client(utility::string_t const& appKey, utility::string_t const& restKey)
	: _MainUri(U("https://api.parse.com"))
	, _AppKey(appKey), _RestKey(restKey)
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

utility::string_t const& parse::api::Client::getMainUri() const {
	return _MainUri;
}

void parse::api::Client::setMainUri(utility::string_t const& uri) {
	_MainUri = uri;
}

void parse::api::Client::fillCommonParseHeaders(web::http::http_request& req)
{
	req.headers().add(U("X-Parse-Application-Id"), _AppKey);
	req.headers().add(U("X-Parse-REST-API-Key"), _RestKey);
}

web::json::value parse::api::Client::requestSync(web::http::http_request const& req)
{
	auto task = _Http.request(req)
		.then([](web::http::http_response response)
	{
		return response.extract_json();
	});

	task.wait();
	return task.get();
}
