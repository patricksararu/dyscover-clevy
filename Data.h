//---------------------------------------------------------------------------

#ifndef DataH
#define DataH

//---------------------------------------------------------------------------

struct Key
{
	int keyCode;
	char soundNormal[7];
	int conf1;
	int conf2;
	int conf3;
	int conf4;
	int conf5;
	int conf6;
	char soundShift[7];
	char soundAlt[7];
	char soundCtrl[7];
};

String GetSoundForKey(int keyCode, bool ctrl, bool shift, bool alt, bool useCover = false);  // What should we sound like
String GetValueForKey(int keyCode, bool ctrl, bool shift, bool alt, bool useCover = false);  // What should we display

#endif
