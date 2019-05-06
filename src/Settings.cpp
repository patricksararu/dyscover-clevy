// ---------------------------------------------------------------------------
#include <vcl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <IOUtils.hpp>
#include <System.SysUtils.hpp>
#include <map>
// ---------------------------------------------------------------------------
#include <locale>
// #include <codecvt>
#include <string>
#include <fstream>
#include <cstdlib>
// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "Settings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------

namespace lokiCpp
{

	TSettings *TSettings::d_instance = nullptr;

	TSettings *TSettings::Instance()
	{
		if (d_instance == nullptr)
		{
			d_instance = new TSettings();
		}
		return d_instance;

	}

	void TSettings::DestroyInstance()
	{
		if (d_instance != nullptr)
		{
			delete d_instance;
			d_instance = nullptr;
		}
	}

	TSettings::TSettings()
		: d_settings(nullptr)
	{
		// load Settingss from file
		d_settingsFileLocation = IncludeTrailingBackslash(ExtractFilePath(Application->ExeName)) + "config\\";

		if (!TDirectory::Exists(d_settingsFileLocation)) {
			TDirectory::CreateDirectory(d_settingsFileLocation);
		}

		d_settingsFiles = TDirectory::GetFiles(d_settingsFileLocation, L"*.conf.json");

		LoadSettings();
      CheckVersionStatus();
	}

	TSettings::~TSettings()
	{
		delete d_settings;
		d_settings = nullptr;
	}

	void TSettings::LoadSettings()
	{
		delete d_settings;
		d_settings = new TJSONObject();

		// std::map<Context, Filename>
		std::map < String, String > ConfigurationFiles;

		for (int idx = 0; idx < d_settingsFiles.Length; ++idx)
		{
			String FileName = ExtractFileName(d_settingsFiles[idx]);
			ConfigurationFiles[FileName.SubString(0, FileName.Pos(".conf.json") - 1)] = d_settingsFiles[idx];
		}

		// this code is taken from stackoverflow to read UTF8 chars using wifstream
		// const std::locale empty_locale = std::locale::empty();

		// works only with 64 bit
		// typedef std::codecvt_utf8 < wchar_t > converter_type;
		// const converter_type *converter   = new converter_type;
		// const std::locale     utf8_locale = std::locale(empty_locale, converter);

		std::map < String, String > ::iterator it = ConfigurationFiles.begin();
		while (it != ConfigurationFiles.end())
		{
			std::wifstream d_fileHandle(it->second.w_str());

			// works only with 64 bit
			// d_fileHandle.imbue(utf8_locale);
			//

			std::wstring line;
			wchar_t chr;
			String json_text = " ";

			if (d_fileHandle.is_open())
			{
				while (d_fileHandle.get(chr))
				{
					if (chr == '\\')
					{
						json_text += '\\';
					}
					json_text += chr;
				}
				// while (std::getline(d_fileHandle, line))
				// {
				// json_text += line.c_str();
				// }
			}

			d_fileHandle.close();

			TJSONObject *context = dynamic_cast < TJSONObject * > (TJSONObject::ParseJSONValue(TEncoding::UTF8->GetBytes(json_text), 0));
			if (context != nullptr)
			{
				d_settings->AddPair(new TJSONPair(new TJSONString(it->first), context));
			}
			else
			{
				throw new Exception(it->first + ".conf.json has an invalid JSON format");
			}
			++it;
		}

	}

	void TSettings::CheckVersionStatus()
	{
		String ApplicationName    = lokiCpp::APPLICATION_NAME;
		String ApplicationVersion = String(lokiCpp::version::app::iMAJOR) + "." + String(lokiCpp::version::app::iMINOR) + "." + String(lokiCpp::version::app::iBUILD);
		String DatabaseVersion    = String(lokiCpp::version::db::iMAJOR) + "." + String(lokiCpp::version::db::iMINOR) + "." + String(lokiCpp::version::db::iBUILD);
		String ApplicationBuildDT = String(__DATE__) + " " + String(__TIME__);

		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue("version"));
		if (context == nullptr)
		{
			// if the context is not found create it..
			context = new TJSONObject();
			context->AddPair(new TJSONString("Application"), new TJSONString("-"));
			context->AddPair(new TJSONString("Version"), new TJSONString("-"));
			context->AddPair(new TJSONString("DatabaseVersion"), new TJSONString("-"));
			context->AddPair(new TJSONString("BuildDate"), new TJSONString("-"));
		}

		// check if all versionStatus is up-to-date.
		if (context->GetValue("Application")->ToString().CompareIC(ApplicationName) != 0 ||
			context->GetValue("Version")->ToString().CompareIC(ApplicationVersion) != 0 ||
			context->GetValue("DatabaseVersion")->ToString().CompareIC(DatabaseVersion) != 0 ||
			context->GetValue("BuildDate")->ToString().CompareIC(ApplicationBuildDT) != 0)
		{
			context->RemovePair("Application");
			context->RemovePair("Version");
			context->RemovePair("DatabaseVersion");
			context->RemovePair("BuildDate");

			context->AddPair(new TJSONString("Application"), new TJSONString(ApplicationName));
			context->AddPair(new TJSONString("Version"), new TJSONString(ApplicationVersion));
			context->AddPair(new TJSONString("DatabaseVersion"), new TJSONString(DatabaseVersion));
			context->AddPair(new TJSONString("BuildDate"), new TJSONString(ApplicationBuildDT));

			d_settings->RemovePair("version");
			d_settings->AddPair("version", context);

         SaveConfig("version");
		}

	}

	bool TSettings::GetConfig(String aContext, String aSubject, String &value)
	{
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// if the context is not found return false.
			return false;
		}

		// context is available.. now search for the subject

		TJSONString *requested = dynamic_cast < TJSONString * > (context->GetValue(aSubject));
		if (requested == nullptr)
		{
			// if the subject is not found return false
			return false;
		}

		value = requested->Value();
		return true; // the context / subject is found. return true

	}

	bool TSettings::GetConfig(String aContext, String aSubject, bool &value)
	{
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// if the context is not found return false.
			return false;
		}

		// context is available.. now search for the subject

		TJSONTrue *requestedTrue = dynamic_cast < TJSONTrue * > (context->GetValue(aSubject));
		TJSONFalse *requestedFalse = dynamic_cast < TJSONFalse * > (context->GetValue(aSubject));
		if (requestedTrue != nullptr)
		{
			value = true;
		}
		else if (requestedFalse != nullptr)
		{
			value = false;
		}
		else
		{
			// if the subject is not found return false
			return false;
		}

		return true; // the context / subject is found. return true
	}

	bool TSettings::GetConfig(String aContext, String aSubject, double &value)
	{
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// if the context is not found return false.
			return false;
		}

		// context is available.. now search for the subject

		TJSONNumber *requested = dynamic_cast < TJSONNumber * > (context->GetValue(aSubject));
		if (requested == nullptr)
		{
			// if the subject is not found return false
			return false;
		}

		value = requested->AsDouble;
		return true; // the context / subject is found. return true
	}

	void TSettings::SetConfig(String aContext, String aSubject, String value)
	{
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// add the context
			context = new TJSONObject();
			d_settings->AddPair(new TJSONPair(new TJSONString(aContext), context));
		}

		// context is available.. now search for the subject
		context->RemovePair(aSubject);

		// add the subject
		context->AddPair(new TJSONPair(new TJSONString(aSubject), new TJSONString(value)));
	}

	void TSettings::SetConfig(String aContext, String aSubject, bool value)
	{
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// add the context
			context = new TJSONObject();
			d_settings->AddPair(new TJSONPair(new TJSONString(aContext), context));
		}

		// context is available.. now search for the subject
		context->RemovePair(aSubject);
		if (value)
		{
			// add the subject
			context->AddPair(new TJSONPair(new TJSONString(aSubject), new TJSONTrue()));
		}
		else
		{
			context->AddPair(new TJSONPair(new TJSONString(aSubject), new TJSONFalse()));
		}

	}

	void TSettings::SetConfig(String aContext, String aSubject, double value)
	{
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// add the context
			context = new TJSONObject();
			d_settings->AddPair(new TJSONPair(new TJSONString(aContext), context));
			d_settingsFiles[d_settingsFiles.Length] = aContext + ".conf.json";
		}

		// context is available.. now search for the subject
		context->RemovePair(aSubject);

		// add the subject
		context->AddPair(new TJSONPair(new TJSONString(aSubject), new TJSONNumber(value)));
	}

	bool TSettings::SaveConfig()
	{
		// foreach TJSONObject call SaveConfig(context)
		for (int idx = 0; idx != d_settings->Count; ++idx)
		{
			TJSONPair *context = dynamic_cast < TJSONPair * > (d_settings->Pairs[idx]);
			String FileSubject = context->JsonString->Value();
			SaveConfig(FileSubject);
		}

		return true;
	}

	bool TSettings::SaveConfig(String aContext)
	{
		{
			std::wofstream d_fileHandle;

			// works only with 64 bit
			// const std::locale empty_locale = std::locale::empty();
			// typedef std::codecvt_utf8 < wchar_t > converter_type;
			// const converter_type *converter   = new converter_type;
			// const std::locale     utf8_locale = std::locale(empty_locale, converter);

			d_fileHandle.open(String(d_settingsFileLocation + L"configuration.json").w_str(), std::ios::out);

			// works only with 64 bit
			// d_fileHandle.imbue(utf8_locale);

			// log is not present when the language is used seperate
			// performLog(lokiCpp::log::eDEBUG, "TLanguage", inst->myTranslations->ToString());

			d_fileHandle << d_settings->ToString().w_str();
			d_fileHandle.close();
		}
		TJSONObject *context = dynamic_cast < TJSONObject * > (d_settings->GetValue(aContext));
		if (context == nullptr)
		{
			// if the context is not found return false.
			return false;
		}

		std::wofstream d_fileHandle;

		// works only with 64 bit
		// const std::locale empty_locale = std::locale::empty();
		// typedef std::codecvt_utf8 < wchar_t > converter_type;
		// const converter_type *converter   = new converter_type;
		// const std::locale     utf8_locale = std::locale(empty_locale, converter);

		d_fileHandle.open(String(d_settingsFileLocation + aContext + L".conf.json").w_str(), std::ios::out);

		// works only with 64 bit
		// d_fileHandle.imbue(utf8_locale);

		// log is not present when the language is used seperate
		// performLog(lokiCpp::log::eDEBUG, "TLanguage", inst->myTranslations->ToString());

		d_fileHandle << context->ToString().w_str();
		d_fileHandle.close();

		return true;
	}

}