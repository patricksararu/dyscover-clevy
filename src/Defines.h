#ifndef DefinesH
#define DefinesH

namespace lokiCpp
{
	static const wchar_t APPLICATION_NAME[] = L"Clevy Dyscover";

	namespace version
	{
		namespace db
		{ // the db version only changes when the stucture changes
			const bool Available = false;
			int const  iMAJOR    = 0; // changes with new/delete Tables/SP/VIEWS
			int const  iMINOR    = 0; // changes with modifications to Tables/SP/VIEWS
			int const  iBUILD    = 0; // changes with internal improvments not changing the interfaces.
		}

	}

	int const WM_MODAL_RESULT_OK = WM_USER + 1;
	int const WM_MODAL_RESULT_CANCEL = WM_USER + 2;
	int const WM_MODAL_RESULT_ERROR  = WM_USER + 3;

}

#endif
