// ---------------------------------------------------------------------------
#ifndef SettingsH
#define SettingsH
// ---------------------------------------------------------------------------
#include <system.hpp>
#include <System.Json.hpp>
// ---------------------------------------------------------------------------
#include "LokiCpp/common/defines.h"
// ---------------------------------------------------------------------------

namespace lokiCpp
{
	class TSettings
	{
		static TSettings *d_instance;

		TJSONObject *   d_settings;

		String          d_settingsFileLocation;
		TStringDynArray d_settingsFiles;

	public:
		static TSettings *Instance();
		static void DestroyInstance();

		bool GetConfig(String aContext, String aSubject, String &value);
		bool GetConfig(String aContext, String aSubject, bool &value);
		bool GetConfig(String aContext, String aSubject, double &value);

		void SetConfig(String aContext, String aSubject, String value);
		void SetConfig(String aContext, String aSubject, bool value);
		void SetConfig(String aContext, String aSubject, double value);

		bool SaveConfig();
		bool SaveConfig(String aContext);
		// TJSONObject *GetSettings(String scope) {return mySettings;}

	private:

		TSettings();
		~TSettings();

		// ! load the language into memory or request the connection
		void LoadSettings();
		void CheckVersionStatus();

	};
}

#endif
