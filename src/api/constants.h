#pragma once
#include "cpprest/details/basic_types.h"

const utility::string_t MAIN_URI = U("https://api.parse.com");
const utility::string_t USERS_URI = U("/1/users");
const utility::string_t FILES_URI = U("/1/files");
const utility::string_t CLASSES_URI = U("/1/classes");

const utility::string_t HEADER_MASTER_KEY = U("X-Parse-Master-Key");
const utility::string_t HEADER_REST_API_KEY = U("X-Parse-REST-API-Key");
const utility::string_t HEADER_SESSION_TOKEN = U("X-Parse-Session-Token");
const utility::string_t HEADER_APPLICATION_ID = U("X-Parse-Application-Id");

const utility::string_t FIELD_TYPE = U("__type");
const utility::string_t FIELD_RESULTS = U("results");
const utility::string_t FIELD_USERNAME = U("username");
const utility::string_t FIELD_PASSWORD = U("password");
const utility::string_t FIELD_OBJECT_ID = U("objectId");
const utility::string_t FIELD_CREATED_AT = U("createdAt");
const utility::string_t FIELD_CLASS_NAME = U("className");
const utility::string_t FIELD_SESSION_TOKEN = U("sessionToken");

const utility::string_t TYPE_FILE = U("File");
const utility::string_t TYPE_POINTER = U("Pointer");
