[Setup]
AppName=Tic-Tac-Toe
AppVersion=1.0
AppPublisher=WoodenNebula
DefaultDirName={autopf}\Tic-Tac-Toe
DefaultGroupName=Tic-Tac-Toe

OutputDir=..\build\installer\
OutputBaseFilename=Tic-Tac-Toe-Setup

SetupIconFile=..\Tic-Tac-Toe\res\Tic-Tac-Toe.ico
UninstallDisplayIcon={app}\res\Tic-Tac-Toe.ico
UninstallDisplayName="Uninstall Tic-Tac-Toe"

Compression=lzma2
SolidCompression=yes

WizardStyle=modern dynamic

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Main executable
Source: "..\build\bin\Release\windows-x86_64\Tic-Tac-Toe\Tic-Tac-Toe.exe"; DestDir: "{app}"; Flags: ignoreversion

; Resource files
Source: "..\Tic-Tac-Toe\res\*"; DestDir: "{app}\res"; Flags: ignoreversion recursesubdirs createallsubdirs

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Icons]
; Start Menu shortcut
Name: "{group}\Tic-Tac-Toe"; Filename: "{app}\Tic-Tac-Toe.exe"; IconFilename: "{app}\res\textures\Tic-Tac-Toe.ico"
; Desktop shortcut
Name: "{autodesktop}\Tic-Tac-Toe"; Filename: "{app}\Tic-Tac-Toe.exe"; Tasks: desktopicon; IconFilename: "{app}\res\textures\Tic-Tac-Toe.ico"

[Run]
Filename: "{app}\Tic-Tac-Toe.exe"; Description: "Launch Tic-Tac-Toe"; Flags: nowait postinstall skipifsilent