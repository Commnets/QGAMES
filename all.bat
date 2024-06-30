Echo --------------------------------------------------------------------
Echo Generating documentation...
cd .\docs
CALL doxygen "QGame Doxy Configuration"
CALL doxygen "QGame Doxy Configuration No SRC"
CALL doxygen "3inarow Doxy Configuration"
CALL doxygen "AticAtac Doxy Configuration"
CALL doxygen "Sabrewulf Doxy Configuration"
CALL doxygen "DWII Doxy Configuration"
CALL doxygen "BSII Doxy Configuration"
CALL doxygen "PacManII Doxy Configuration"
cd ..\
Echo --------------------------------------------------------------------

Echo --------------------------------------------------------------------
Echo Package the documentation...
cd .\docs
DEL .\No\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\No\html.zip .\No\html
DEL .\3inarow\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\3inarow\html.zip .\3inarow\html
COPY ".\3inarow\html.zip" "..\setups\3inarow\"
DEL .\AticAtac\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\AticAtac\html.zip .\AticAtac\html
COPY ".\AticAtac\html.zip" "..\setups\AticAtac\"
DEL .\Sabrewulf\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\Sabrewulf\html.zip .\Sabrewulf\html
COPY ".\Sabrewulf\html.zip" "..\setups\SabreWulf\"
DEL .\DWII\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\DWII\html.zip .\DWII\html
COPY ".\DWII\html.zip" "..\setups\DragonWindII\"
DEL .\BSII\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\BSII\html.zip .\BSII\html
COPY ".\BSII\html.zip" "..\setups\BattleshipII\"
DEL .\PacManII\html.zip /Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\PacManII\html.zip .\PacManII\html
COPY ".\PacManII\html.zip" "..\setups\PacManII\"
cd ..\
Echo --------------------------------------------------------------------

Echo --------------------------------------------------------------------
Echo Copy samples to their place...
cd .\samples
CALL "ActualizeInfo"
cd ..\
Echo --------------------------------------------------------------------

Echo --------------------------------------------------------------------
Echo Copy executables and code for games to many places...
cd .\games
CALL "ActualizeInfo"
cd ..\
Echo --------------------------------------------------------------------

Echo --------------------------------------------------------------------
Echo Execute setups...
cd .\setups
CALL "setups"
cd ..\
Echo --------------------------------------------------------------------

Echo --------------------------------------------------------------------
Echo The final complied results are copied into the cds directory...
COPY ".\setups\AticAtac\output\*.exe" .\cds
COPY ".\setups\SabreWulf\output\*.exe" .\cds
COPY ".\setups\3inarow\output\*.exe" .\cds
COPY ".\setups\DragonwindII\output\*.exe" .\cds
COPY ".\setups\BattleshipII\output\*.exe" .\cds
COPY ".\setups\PacManII\output\*.exe" .\cds
cd .\docs
COPY ".\No\html.zip" "..\cds\html.zip"
COPY ".\3inarow\html.zip" "..\cds\docs\3inarowHTML.zip"
COPY ".\AticAtac\html.zip" "..\cds\docs\AticAtacHTML.zip"
COPY ".\Sabrewulf\html.zip" "..\cds\docs\SabrewulfHTML.zip"
COPY ".\DWII\html.zip" "..\cds\docs\DWIIHTML.zip"
COPY ".\BSII\html.zip" "..\cds\docs\BSIIHTML.zip"
COPY ".\PacManII\html.zip" "..\cds\docs\PacManIIHTML.zip"
cd ..\
Echo --------------------------------------------------------------------

Echo --------------------------------------------------------------------
Echo The setup for the QGAMES environment
COPY ".\docs\No\html.zip" ".\setups\QGAMES\docs"
DEL ".\setups\QGAMES\include\*.*" /S/Q
RMDIR ".\setups\QGAMES\include" /S/Q
XCOPY ".\include\*.*" ".\setups\QGAMES\include\" /s/q
DEL ".\setups\QGAMES\lib\*.*" /S/Q
COPY ".\lib\*.lib" ".\setups\QGAMES\lib"
DEL ".\setups\QGAMES\projects\*.*" /S/Q
RMDIR ".\setups\QGAMES\projects" /S/Q
XCOPY ".\projects\tinyxml\*.h" ".\setups\QGAMES\projects\tinyxml\" /s
XCOPY ".\projects\tmxparser\*.h" ".\setups\QGAMES\projects\tmxparser\" /s
XCOPY ".\projects\zpplib\*.h" ".\setups\QGAMES\projects\zpplib\" /s
DEL ".\setups\QGAMES\samples\*.*" /S/Q
RMDIR ".\setups\QGAMES\samples" /S/Q
RMDIR ".\setups\QGAMES\templates" /S/Q
RMDIR ".\setups\QGAMES\commonlibs" /S/Q
XCOPY ".\samples\TestAdArcade\*.*" ".\setups\QGAMES\samples\TestAdArcade\" /s
XCOPY ".\samples\TestPlatformGame\*.*" ".\setups\QGAMES\samples\TestPlatformGame\" /s
XCOPY ".\samples\TestDragonwind\*.*" ".\setups\QGAMES\samples\TestDragonwind\" /s
XCOPY ".\samples\TestBattleship\*.*" ".\setups\QGAMES\samples\TestBattleship\" /s
XCOPY ".\samples\TestPacMan\*.*" ".\setups\QGAMES\samples\TestPacMan\" /s
XCOPY ".\samples\HelloWorld\*.*" ".\setups\QGAMES\samples\HelloWorld\" /s
XCOPY ".\samples\HelloWorld2\*.*" ".\setups\QGAMES\samples\HelloWorld2\" /s
XCOPY ".\templates\*.*" ".\setups\QGAMES\templates\" /s
XCOPY ".\commonlibs\*.*" ".\setups\QGAMES\commonlibs\" /s
COPY ".\Tests.sln" ".\setups\QGAMES"
COPY ".\READMETests.rtf" ".\setups\QGAMES\README.rtf"
DEL ".\setups\QGAMES\output\*.*" /S/Q
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESLib.zip @.\setups\QGAMES\list1.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESProjects.zip @.\setups\QGAMES\list2.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESSamples1.zip @.\setups\QGAMES\list3.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESSamples2.zip @.\setups\QGAMES\list4.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESSamples3.zip @.\setups\QGAMES\list5.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESSamples4.zip @.\setups\QGAMES\list6.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESSamples5.zip @.\setups\QGAMES\list7.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESSamples6.zip @.\setups\QGAMES\list8.txt
CALL "C:\Program Files\7-Zip\7z.exe" a -tzip .\setups\QGAMES\output\QGAMESCommonLibs.zip @.\setups\QGAMES\list9.txt
COPY .\setups\QGAMES\templates\*.zip .\setups\QGAMES\output
COPY .\setups\QGAMES\docs\*.zip .\setups\QGAMES\output
COPY .\setups\QGAMES\Tests.sln .\setups\QGAMES\output
COPY .\setups\QGAMES\README.rtf .\setups\QGAMES\output
Echo --------------------------------------------------------------------
