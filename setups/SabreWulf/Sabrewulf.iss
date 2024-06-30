; License Generator Installer
; Copyright 2016 by Community Networks S.L.
; Ignacio Cea Forniés

#define MyAppName "SabreWulf"
#define MyAppExeName "Sabrewulf.exe"
#define MyProductName "Community Networks Sabrewulf"
#define MyProductInfo "Community Networks Sabrewulf"
#include "..\Commons\Commons.iss"

[Setup]
AppId={{30598836-90A5-43DE-A178-4E8390F5D44C}
SetupIconFile=Sabreman.ico
UninstallDisplayIcon={app}\Sabreman.ico

[Files]
Source: "resources.icf"; DestDir: "{app}"; Flags: ignoreversion
Source: "sabremap.jpg"; DestDir: "{app}"; Flags: ignoreversion
Source: "sabrewulfmap(1).png"; DestDir: "{app}"; Flags: ignoreversion
Source: "sabrewulfmap(2).png"; DestDir: "{app}"; Flags: ignoreversion
Source: "Sabrewulf.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "conf.xml"; DestDir: "{app}"; Flags: ignoreversion; AfterInstall: configureConf
Source: "Sabreman.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "html.zip"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "code\*.*"; DestDir: "{app}\code"; Flags: ignoreversion

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Run]

[Dirs]
Name: "{app}\code";
Name: "{code:getDataDir}";
Name: "{code:getDataDir}\forms";
Name: "{code:getDataDir}\maps";
Name: "{code:getDataDir}\sounds";
Name: "{code:getDataDir}\objects";

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
  
  { 
    addLicensePage;
    This page defined in Commons.iss, 
    but it's been disabled in this installation.
  }
  
  addDataDirPage; { Defined in Common.iss }
  addScalePage; { Defined in Common.iss }
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
  
  If (curPageId = _dataDirPage.ID) Then
  Begin
    _message := validateDataDirAtNextButtonClick ();
  End
  Else
  If (curPageId = _scalePage.ID) Then
  Begin
    _message := validateScaleAtNextButtonClick ();
  End;
  
  { This versión doesn't consider asking for a license file
    So this part of the code has been commented 
    Else
    If (curPageId = _licensePage.ID) Then
      _message := validateLicenseFileAtNextButtonClick ();
  }

  If (_message <> '') Then
  Begin
    MsgBox (_message, mbError, MB_OK);
    Result := False;
  End
  Else
    Result := True;

End;

{ Event shoot when the instalation of Conf.xml finishes. }
Procedure configureConf;
Begin

  { Configure the the data dir }
  changeTextInFile (ExpandConstant('{app}')+'\Conf.xml', '@@DATA_DIR@@', 
    _dataDirPage.Values [0]);

  { Configure the scale of the graphics system }
  changeTextInFile (ExpandConstant('{app}')+'\Conf.xml', '@@SCALE@@', 
    _scalePage.Values [0]);

End;

{ End of code }
