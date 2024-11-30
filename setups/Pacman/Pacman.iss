; License Generator Installer
; Copyright 2013 by Community Networks S.L.
; Ignacio Cea Forniés

#define MyAppName "Pacman"
#define MyAppExeName "Pacman.exe"
#define MyProductName "Community Networks Pacman"
#define MyProductInfo "Community Networks Pacman"
#include "..\Commons\Commons.iss"

[Setup]
AppId={{5FC288BA-FA34-45B3-9CAB-3A72A44536BC}
SetupIconFile=Pacman.ico
VersionInfoVersion=1.0.000
VersionInfoProductVersion=1.0.000
UninstallDisplayIcon={app}\Pacman.ico

[Files]
Source: "forms\chompersprites.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\fruits.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\NumbersLittle.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\Ready.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\Text1.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\Text2.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\background.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\Commnets1.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\CrackFont.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\pacmanico.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\pacmanico.ico"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "forms\giantpacman.png"; DestDir: "{app}\forms\"; Flags: ignoreversion
Source: "maps\chompermazetiles.png"; DestDir: "{app}\maps\"; Flags: ignoreversion
Source: "maps\fruits.png"; DestDir: "{app}\maps\"; Flags: ignoreversion
Source: "maps\map.tmx"; DestDir: "{app}\maps\"; Flags: ignoreversion
Source: "sounds\pacman_beginning.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_chomp.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_death.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_eatfruit.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_eatghost.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_extrapac.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_intermission.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "sounds\pacman_siren.wav"; DestDir: "{app}\sounds\"; Flags: ignoreversion
Source: "conf.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "entities.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "forms.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "maps.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "movements.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "objects.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "PacMan.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Pacman.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "Pacman.iss"; DestDir: "{app}"; Flags: ignoreversion
Source: "Pacman.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "sounds.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "worlds.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.txt"; DestDir: "{app}"; Flags: ignoreversion

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Run]

[Dirs]
Name: "{app}\forms"
Name: "{app}\maps"
Name: "{app}\sounds"
Name: "{app}\objects"

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
