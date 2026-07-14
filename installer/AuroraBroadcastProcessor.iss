#ifndef MyAppVersion
  #error MyAppVersion must be defined from VERSION.txt by the packaging script
#endif
#ifndef SourceRoot
  #define SourceRoot ".."
#endif
#ifndef OutputDir
  #define OutputDir "..\dist"
#endif

[Setup]
AppId={{13E446AB-8FC2-4F8A-A077-A6E6B8615A4A}
AppName=Aurora Broadcast Processor
AppVersion={#MyAppVersion}
AppPublisher=Ammar Audio Labs
AppPublisherURL=https://example.invalid/aurora-bp
AppSupportURL=https://example.invalid/aurora-bp/support
DefaultDirName={autopf}\Ammar Audio Labs\Aurora Broadcast Processor
DefaultGroupName=Ammar Audio Labs\Aurora Broadcast Processor
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
OutputDir={#OutputDir}
OutputBaseFilename=AuroraBroadcastProcessor-Setup-v{#MyAppVersion}
Compression=lzma2/max
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\Aurora Broadcast Processor.exe
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany=Ammar Audio Labs
VersionInfoDescription=Aurora Broadcast Processor Windows x64 installer
VersionInfoProductName=Aurora Broadcast Processor
VersionInfoProductVersion={#MyAppVersion}
LicenseFile={#SourceRoot}\installer\licence.txt
MinVersion=10.0

[Tasks]
Name: "startmenu"; Description: "Create a Start menu shortcut"; Flags: checkedonce
Name: "desktopicon"; Description: "Create a desktop shortcut"; Flags: unchecked

[Dirs]
Name: "{commoncf64}\VST3\Aurora Broadcast Processor.vst3"

[Files]
Source: "{#SourceRoot}\build\windows-x64\AuroraBroadcastProcessor_artefacts\Release\VST3\Aurora Broadcast Processor.vst3\*"; DestDir: "{commoncf64}\VST3\Aurora Broadcast Processor.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#SourceRoot}\build\windows-x64\AuroraBroadcastProcessor_artefacts\Release\Standalone\Aurora Broadcast Processor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceRoot}\README.md"; DestDir: "{app}\Documentation"; Flags: ignoreversion
Source: "{#SourceRoot}\docs\INSTALLATION.md"; DestDir: "{app}\Documentation"; Flags: ignoreversion
Source: "{#SourceRoot}\docs\HOST_TESTING.md"; DestDir: "{app}\Documentation"; Flags: ignoreversion
Source: "{#SourceRoot}\LICENSE_NOTICE.md"; DestDir: "{app}\Documentation"; Flags: ignoreversion
Source: "{#SourceRoot}\THIRD_PARTY_NOTICES.md"; DestDir: "{app}\Documentation"; Flags: ignoreversion

[Icons]
Name: "{group}\Aurora Broadcast Processor"; Filename: "{app}\Aurora Broadcast Processor.exe"; Tasks: startmenu
Name: "{autodesktop}\Aurora Broadcast Processor"; Filename: "{app}\Aurora Broadcast Processor.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Aurora Broadcast Processor.exe"; Description: "Launch Aurora Broadcast Processor"; Flags: nowait postinstall skipifsilent
