// ---------------------------------------------------------------------------

#ifndef LanguageH
#define LanguageH
// ---------------------------------------------------------------------------
#include <system.hpp>
#include <System.Json.hpp>

#include "LokiCpp/common/defines.h"
// ---------------------------------------------------------------------------
namespace lokiCpp
{
	class TLanguage
	{
		static TLanguage *d_instance;

		String d_languageFilesLocation;
		String d_languageFileName;

		TStringDynArray d_languageFiles;

		TJSONObject *myTranslations;

		int iSetLanguage;

	public:
		// ! this class will only present a static interface
		static bool TranslationAvailable(String Category, String Subject);
		static String Translate(String Category, String Subject);
		static String AddTranslation(String Category, String Subject, String Translation);

		static void SetLanguage(String LanguageCode);

		TJSONObject *getTranslations() {return myTranslations;}

	private:
		static TLanguage *Instance();
		static void DestroyInstance();

		TLanguage();
		TLanguage(TLanguage const &other);
		~TLanguage();

		// ! load the language into memory or request the connection
		void LoadLanguage(String LanguageCode);

	};
}

#endif
