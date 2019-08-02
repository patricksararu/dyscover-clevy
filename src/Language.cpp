// ---------------------------------------------------------------------------
#include <vcl.h>
#include <IOUtils.hpp>
#include <fstream>
#include <string>
// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "Language.h"
#include "Settings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------

namespace lokiCpp
{

	TLanguage *TLanguage::d_instance = nullptr;

	TLanguage *TLanguage::Instance()
	{
		if (d_instance == nullptr)
		{
			d_instance = new TLanguage();
		}
		return d_instance;

	}

	void TLanguage::DestroyInstance()
	{
		if (d_instance != nullptr)
		{
			delete d_instance;
			d_instance = nullptr;
		}
	}

	TLanguage::TLanguage()
		: myTranslations(nullptr)
	{
		// load languages from file
		d_languageFilesLocation = IncludeTrailingBackslash(ExtractFilePath(Application->ExeName)) + "Language\\";

		if (!TDirectory::Exists(d_languageFilesLocation)) {
			TDirectory::CreateDirectory(d_languageFilesLocation);
		}

		d_languageFiles = TDirectory::GetFiles(d_languageFilesLocation, L"*.lang.json");

		String LangCode = L"nl"; // set the language code default to en.. if nothing exists we continue in English

		String sValue = "";
		if (TSettings::Instance()->GetConfig("general", "DefaultGuiLanguage", sValue))
		{
			LangCode = sValue;
		}

		LoadLanguage(LangCode);
	}

	TLanguage::TLanguage(TLanguage const &other)
		: myTranslations(other.myTranslations),
		  d_languageFilesLocation(other.d_languageFilesLocation),
		  d_languageFileName(other.d_languageFileName)
	{
	}

	TLanguage::~TLanguage()
	{
		delete myTranslations;
		myTranslations = nullptr;
	}

	void TLanguage::SetLanguage(String LanguageCode)
	{
		TLanguage *inst = TLanguage::Instance();

		inst->LoadLanguage(LanguageCode);
	}

	void TLanguage::LoadLanguage(String LanguageCode)
	{
		d_languageFileName = d_languageFilesLocation + LanguageCode + ".lang.json";

		for (int idx = 0; idx < d_languageFiles.Length; ++idx)
		{
			if (d_languageFiles[idx].SubString(0, 2).CompareIC(LanguageCode) == 0)
			{
				d_languageFileName = d_languageFilesLocation + d_languageFiles[idx];
			}
		}

		// this code is taken from stackoverflow to read UTF8 chars using wifstream
		// const std::locale empty_locale = std::locale::empty();

		// works only with 64 bit
		// typedef std::codecvt_utf8 < wchar_t > converter_type;
		// const converter_type *converter   = new converter_type;
		// const std::locale     utf8_locale = std::locale(empty_locale, converter);

		std::wifstream d_fileHandle(d_languageFileName.w_str());

		// works only with 64 bit
		// d_fileHandle.imbue(utf8_locale);
		//

		std::wstring line;
		String json_text = " ";

		if (d_fileHandle.is_open())
		{
			while (std::getline(d_fileHandle, line))
			{
				json_text += line.c_str();
			}
		}

		d_fileHandle.close();

		delete myTranslations;
		myTranslations = dynamic_cast < TJSONObject * > (TJSONObject::ParseJSONValue(TEncoding::UTF8->GetBytes(json_text), 0));

	}

	bool TLanguage::TranslationAvailable(String Category, String Subject)
	{
		TLanguage *inst = TLanguage::Instance();

		if (inst->myTranslations != nullptr)
		{
			if (inst->myTranslations->GetValue(Category) != nullptr)
			{
				TJSONObject *sub = dynamic_cast < TJSONObject * > (inst->myTranslations->GetValue(Category));
				if (sub != nullptr && sub->GetValue(Subject) != nullptr)
				{
					return true;
				}

			}
		}
		return false;
	}

	String TLanguage::Translate(String Category, String Subject)
	{
		TLanguage *inst = TLanguage::Instance();

		if (inst->myTranslations != nullptr)
		{
			if (inst->myTranslations->GetValue(Category) != nullptr)
			{
				TJSONObject *sub = dynamic_cast < TJSONObject * > (inst->myTranslations->GetValue(Category));
				if (sub != nullptr && sub->GetValue(Subject) != nullptr)
				{
					return sub->GetValue(Subject)->Value();
				}

			}
		}
		AddTranslation(Category, Subject, Subject);
		return " < " + Category + " : " + Subject + " > ";
	}

	String TLanguage::AddTranslation(String Category, String Subject, String Translation)
	{
		// log is not present when the language is used seperate
		// performLog(lokiCpp::log::eDEBUG, "TLanguage", "Add Translatrion for : " + Category + " : " + Subject + " => " + Translation);
		TLanguage *inst = TLanguage::Instance();

		if (inst->myTranslations == nullptr)
		{
			inst->myTranslations = new TJSONObject();
		}
		if (inst->myTranslations->GetValue(Category) == nullptr)
		{
			TJSONPair *myPair  = new TJSONPair(new TJSONString(Category),new TJSONObject());

			inst->myTranslations->AddPair(myPair);
		}

		TJSONObject *sub = dynamic_cast < TJSONObject * > (inst->myTranslations->GetValue(Category));

		TJSONPair *myPair  = new TJSONPair(new TJSONString(Subject),new TJSONString(Translation));

		sub->AddPair(myPair);

		std::wofstream d_fileHandle;

		// works only with 64 bit
		// const std::locale empty_locale = std::locale::empty();
		// typedef std::codecvt_utf8 < wchar_t > converter_type;
		// const converter_type *converter   = new converter_type;
		// const std::locale     utf8_locale = std::locale(empty_locale, converter);

		d_fileHandle.open(inst->d_languageFileName.w_str(), std::ios::out);

		// works only with 64 bit
		// d_fileHandle.imbue(utf8_locale);

		// log is not present when the language is used seperate
		// performLog(lokiCpp::log::eDEBUG, "TLanguage", inst->myTranslations->ToString());

		d_fileHandle << inst->myTranslations->ToString().w_str();
		d_fileHandle.close();

		return " < " + Category + " : " + Subject + " > ";
	}

}
