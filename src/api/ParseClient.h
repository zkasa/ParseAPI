#pragma once
#include "cpprest/details/basic_types.h"
#include "cpprest/http_client.h"

namespace parse {
namespace api {

class Client
{
public:
	Client(utility::string_t const& appKey, utility::string_t const& restKey);
	~Client();

	utility::string_t const& getAppKey() const;
	utility::string_t const& getRestKey() const;
	utility::string_t const& getMainUri() const;
	void setMainUri(utility::string_t const& uri);

	void fillCommonParseHeaders(web::http::http_request& req);
	web::json::value requestSync(web::http::http_request const& req);

private:
	utility::string_t _AppKey;
	utility::string_t _RestKey;
	utility::string_t _MainUri;
	web::http::client::http_client _Http;
};

}
}