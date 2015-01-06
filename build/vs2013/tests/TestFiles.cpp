#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "keys.h"
#include "ParseClient.h"
#include "ParseObjects.h"
#include "ParseFiles.h"


namespace tests
{
	TEST_CLASS(TestFiles)
	{
	public:
		
		TEST_METHOD(UploadFile)
		{
			try {
				parse::api::Client client(cParseAppKey, cParseRestKey, cParseMasterKey);
				parse::api::Files files(client);

				auto file = files.uploadFile(U(R"(..\tests\test.7z)"));
				Assert::IsTrue(file.isValid());

				auto listFiles = files.getFiles();
				Assert::IsTrue(listFiles.size() == 1);

				for (auto f : listFiles) {
					Assert::IsTrue(files.deleteFile(f));
				}
			}
			catch (std::exception const& ex) {
				Assert::Fail(utility::conversions::to_string_t(ex.what()).c_str());
			}
		}

		TEST_METHOD(QueryFile)
		{
			try {
				parse::api::Client client(cParseAppKey, cParseRestKey, cParseMasterKey);
				parse::api::Objects objs(client, U("FileOwner"));
				parse::api::Files files(client);

				auto file1 = files.uploadFile(U(R"(..\tests\test.7z)"));
				Assert::IsTrue(file1.isValid());

				auto obj = objs.createObject();
				Assert::IsTrue(obj.isValid());

				auto file2 = files.uploadFile(U(R"(..\tests\test.7z)"), obj, U("owner"));
				Assert::IsTrue(file2.isValid());

				auto listFiles = files.getFilesOf(obj, U("owner"));
				Assert::IsTrue(listFiles.size() == 1);

				Assert::IsTrue(files.deleteFile(file1));
				Assert::IsTrue(files.deleteFile(file2));
				objs.deleteObject(obj);
				Assert::IsFalse(obj.isValid());
			}
			catch (std::exception const& ex) {
				Assert::Fail(utility::conversions::to_string_t(ex.what()).c_str());
			}
		}
	};
}