; Common for both 32-bit and 64-bit version

#define MyAppName "Clevy Dyscover"
#define MyAppVersion GetFileVersion(ClevyExePath)
#define MyAppPublisher "BNC Distribution"
#define MyAppURL "http://www.clevy.com/"
#define MyAppExeName "Clevy.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B1128504-2C33-4950-AFFA-D743268EB035}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\Clevy Dyscover.new
DisableProgramGroupPage=yes
VersionInfoVersion={#MyAppVersion}
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=Output
SetupIconFile=..\res\ClevyIcon.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: {app}; permissions: everyone-full
Name: {app}\config; Permissions: everyone-full
Name: {app}\data; Permissions: everyone-full
Name: {app}\language; Permissions: everyone-full
Name: {app}\data\tts
Name: {app}\data\tts\data
Name: {app}\data\tts\data\sfx
Name: {app}\data\tts\data\sfx\voice
Name: {app}\data\tts\data\sfx\voice\Ilse

[Files]
Source: {#ClevyExePath}; DestDir: "{app}"; DestName: {#MyAppExeName}; Flags: ignoreversion
Source: {#RsttsDllPath}; DestDir: "{app}"; Flags: ignoreversion
Source: "..\res\language\*.lang.json"; DestDir: "{app}\language"; Flags: ignoreversion
Source: "..\res\data\*"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallRun]
Filename: "taskkill"; Parameters: "/im {#MyAppExeName} /f"; Flags: runhidden

[UninstallDelete]
Type: filesandordirs; Name: "{app}\config"
