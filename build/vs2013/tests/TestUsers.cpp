#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "keys.h"
#include "ParseClient.h"
#include "ParseUsers.h"

namespace tests
{
	TEST_CLASS(TestUsers)
	{
	public:
		
		TEST_METHOD(CreateAndLoginUser)
		{
			parse::api::Client client(cParseAppKey, cParseRestKey, cParseMasterKey);
			parse::api::Users users(client);

			auto lu1 = users.loginUser(U("acsa"), U("wevw"));
			Assert::IsFalse(lu1.isValid() && lu1.isLoggedIn());

			auto u = users.createUser(U("motorola"), U("horror"));
			Assert::IsTrue(u.isValid() /*&& !u.isLoggedIn()*/);

			// correct login
			auto lu2 = users.loginUser(U("motorola"), U("horror"));
			Assert::IsTrue(lu2.isValid() && lu2.isLoggedIn());

			users.deleteUser(u);
		}

		TEST_METHOD(ValidateUserSession)
		{
			parse::api::Client client(cParseAppKey, cParseRestKey, cParseMasterKey);
			parse::api::Users users(client);

			auto u = users.createUser(U("givi"), U("death"));
			Assert::IsTrue(u.isValid() /*&& !u.isLoggedIn()*/);

			// incorrect login
			auto lu1 = users.loginUser(U("givi"), U("peace"));
			Assert::IsFalse(lu1.isValid() && lu1.isLoggedIn());

			auto vu1 = users.validateSession(lu1);
			// after incorrect login must be invalid
			Assert::IsFalse(vu1.isValid());

			// correct login
			auto lu2 = users.loginUser(U("givi"), U("death"));
			Assert::IsTrue(lu2.isValid() && lu2.isLoggedIn());

			auto vu2 = users.validateSession(lu2);
			// after login must be valid
			Assert::IsTrue(vu2.isValid());

			users.deleteUser(u);

		}

	};
}