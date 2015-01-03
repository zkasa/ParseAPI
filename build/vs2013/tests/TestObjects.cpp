#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "keys.h"
#include "ParseClient.h"
#include "ParseObjects.h"

namespace tests
{
	TEST_CLASS(TestObjects)
	{
	public:
		
		TEST_METHOD(CreateObjects)
		{
			parse::api::Client client(cParseAppKey, cParseRestKey);
			parse::api::Objects objects(client, U("TestClass"));

			auto obj = objects.createObject(web::json::value::null());
			Assert::IsFalse(obj.getId().empty());

			objects.deleteObject(obj);
			Assert::IsFalse(obj.isValid());
		}

		TEST_METHOD(GettingObjects)
		{
			parse::api::Client client(cParseAppKey, cParseRestKey);
			parse::api::Objects objects(client, U("TestClass"));

			auto obj1 = objects.createObject(web::json::value::null());
			Assert::IsFalse(obj1.getId().empty());

			auto obj2 = objects.createObject(web::json::value::null());
			Assert::IsFalse(obj2.getId().empty());

			auto arr = objects.getObjects();
			Assert::IsTrue(arr.size() == 2);

			for (auto o : arr) {
				Assert::IsTrue(o.isValid());
			}

			objects.deleteObject(obj1);
			Assert::IsFalse(obj1.isValid());
			objects.deleteObject(obj2);
			Assert::IsFalse(obj2.isValid());
		}

		TEST_METHOD(QueryingObjects)
		{
			try {
				parse::api::Client client(cParseAppKey, cParseRestKey);
				parse::api::Objects objects(client, U("TestClass"));

				auto obj1 = objects.createObject(U("{ \"name\" : \"Object 1\" }"));
				Assert::IsFalse(obj1.getId().empty());

				auto obj2 = objects.createObject(U("{ \"name\" : \"Object 2\" }"));
				Assert::IsFalse(obj2.getId().empty());

				auto arr = objects.getObjects(U(R"(where={"name":"Object 2"})"));
				Assert::IsTrue(arr.size() == 1);

				Assert::IsTrue(arr[0].getId() == obj2.getId());

				objects.deleteObject(obj1);
				Assert::IsFalse(obj1.isValid());
				objects.deleteObject(obj2);
				Assert::IsFalse(obj2.isValid());
			}
			catch (web::uri_exception const& ex) {
				Assert::Fail(L"web::uri_exception"/*ex.what()*/);
			}
		}


	};
}