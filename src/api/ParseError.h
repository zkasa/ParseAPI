#pragma once
#include "ParseObjects.h"

namespace parse {
namespace api {

class Error : public parse::api::Object
{
public:
	Error(web::json::value const& json);
	~Error();

public:
	bool isError() const;
	utility::string_t getErrorCode() const;
	utility::string_t getErrorMessage() const;
};

}
}