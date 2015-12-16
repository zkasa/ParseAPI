#include "ParseClient.h"
#include "ParseObjects.h"

#include "ParseError.h"

using namespace parse::api;

namespace {
	const utility::string_t FIELD_ERROR = U("error");
	const utility::string_t FIELD_CODE = U("code");
}

Error::Error(web::json::value const& json)
	: Object(U("error"), json)
{
}


Error::~Error()
{
}

bool parse::api::Error::isError() const {
	return hasField(FIELD_ERROR) && !getField(FIELD_ERROR).empty();
}

utility::string_t parse::api::Error::getErrorCode() const {
	return getField(FIELD_CODE);
}

utility::string_t parse::api::Error::getErrorMessage() const {
	return getField(FIELD_ERROR);
}
