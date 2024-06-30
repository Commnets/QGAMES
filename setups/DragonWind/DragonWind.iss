; License Generator Installer
; Copyright 2013 by Community Networks S.L.
; Ignacio Cea Forniés

#define MyAppName "DragonWind"
#define MyAppVersion "1.0.000"
#define MyAppExeName "DragonWind.exe"
#define MyProductName "Community Networks DragonWind"
#define MyProductInfo "Community Networks DragonWind"
#include "..\Commons\Commons.iss"

[Setup]
AppId={{F68BFD86-87FC-41C0-8154-38F64908516F}
SetupIconFile=DragonIcon.ico
VersionInfoVersion=1.0.000
VersionInfoProductVersion=1.0.000
UninstallDisplayIcon={app}\DragonWind.ico

[Files]
Source: "conf.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "resources1.icf"; DestDir: "{app}"; Flags: ignoreversion
Source: "resources2.icf"; DestDir: "{app}"; Flags: ignoreversion
Source: "DragonWind.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "DragonIcon.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "DragonWind.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.txt"; DestDir: "{app}"; Flags: ignoreversion

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Run]

[Dirs]
Name: "{app}\temp\forms"
Name: "{app}\temp\maps"
Name: "{app}\temp\sounds"
Name: "{app}\temp\objects"

[CustomMessages]

[Icons]

[Code]
{ Begin of code }

{ To install the special pages used by this install program.
  Those pages are to ask for information the installation needs }
Procedure createCustomWizardPages;
Begin
  
End;

{ The Initialize Wizard is an event launched
  when the setup program starts. This situation
  is here used to add special wizards to ask for the
  information that the server installation needs. }
Procedure InitializeWizard;
Begin

  CreateAboutButtonAndURLLabel (WizardForm, WizardForm.CancelButton);
  addLicensePage; { Defined in Common.iss }
  createCustomWizardPages;

End;

{ To validate the local data introduced.
  This method is called from the common.iss page, in the NexButtonClick Event.
  Depending on the page you are, the data are validate.
  Once all data is valid, it is completed if needed. }
Function NextButtonClick (curPageId: Integer) : Boolean;
Var
  _message : String;

Begin

  _message := '';
  Result := True;

End;

{ End of code }
