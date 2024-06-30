@Echo OFF

Echo Copy AticAtac
Echo ------------------------
copy "..\exe\AticAtac.exe" ".\AticAtac\"
copy "..\exe\AticAtacD.exe" ".\AticAtac\"

Echo Copy AticAtac to installer
Echo ------------------------
copy "..\exe\AticAtac.exe" "..\setups\AticAtac\"
copy ".\AticAtac\resources.icf" "..\setups\AticAtac\"

Echo Copy code to produce AticAtac to git
Echo ------------------------
DEL ..\products\AticAtac\code\*.hpp
DEL ..\products\AticAtac\code\*.cpp
DEL ..\products\AticAtac\code\*.xml
DEL ..\products\AticAtac\forms\*.* /Q
DEL ..\products\AticAtac\maps\*.* /Q
DEL ..\products\AticAtac\sounds\*.* /Q
DEL ..\setups\AticAtac\code\*.hpp
DEL ..\setups\AticAtac\code\*.cpp
DEL ..\setups\AticAtac\code\*.xml
copy ".\AticAtac\*.hpp" "..\products\AticAtac\code\"
copy ".\AticAtac\*.cpp" "..\products\AticAtac\code\"
copy ".\AticAtac\*.xml" "..\products\AticAtac\code\"
copy ".\AticAtac\forms\*.*" "..\products\AticAtac\forms\"
copy ".\AticAtac\maps\*.*" "..\products\AticAtac\maps\"
copy ".\AticAtac\sounds\*.*" "..\products\AticAtac\sounds\"
copy ".\AticAtac\*.hpp" "..\setups\AticAtac\code\"
copy ".\AticAtac\*.cpp" "..\setups\AticAtac\code\"
copy ".\AticAtac\*.xml" "..\setups\AticAtac\code\"
copy ".\AticAtac\AticAtac.vcxproj" "..\products\AticAtac\code\"
copy ".\AticAtac\AticAtac.vcxproj.filters" "..\products\AticAtac\code\"
copy ".\AticAtac\AticAtac.vcxproj.user" "..\products\AticAtac\code\"
copy ".\AticAtac\AticAtac.vcxproj" "..\setups\AticAtac\code\"
copy ".\AticAtac\AticAtac.vcxproj.filters" "..\setups\AticAtac\code\"
copy ".\AticAtac\AticAtac.vcxproj.user" "..\setups\AticAtac\code\"

Echo Copy SabreWulf
Echo ------------------------
copy "..\exe\Sabrewulf.exe" ".\Sabrewulf\"
copy "..\exe\SabrewulfD.exe" ".\Sabrewulf\"

Echo Copy SabreWulf to installer
Echo ------------------------
copy "..\exe\Sabrewulf.exe" "..\setups\Sabrewulf\"
copy ".\SabreWulf\resources.icf" "..\setups\Sabrewulf\"

Echo Copy code to produce Sabrewulf to git
Echo ------------------------
DEL ..\products\Sabrewulf\code\*.hpp
DEL ..\products\Sabrewulf\code\*.cpp
DEL ..\products\Sabrewulf\code\*.xml
DEL ..\products\Sabrewulf\forms\*.* /Q
DEL ..\products\Sabrewulf\maps\*.* /Q
DEL ..\products\Sabrewulf\sounds\*.* /Q
DEL ..\setups\Sabrewulf\code\*.hpp
DEL ..\setups\Sabrewulf\code\*.cpp
DEL ..\setups\Sabrewulf\code\*.xml
copy ".\Sabrewulf\*.hpp" "..\products\Sabrewulf\code\"
copy ".\Sabrewulf\*.cpp" "..\products\Sabrewulf\code\"
copy ".\Sabrewulf\*.xml" "..\products\Sabrewulf\code\"
copy ".\Sabrewulf\forms\*.*" "..\products\Sabrewulf\forms\"
copy ".\Sabrewulf\maps\*.*" "..\products\Sabrewulf\maps\"
copy ".\Sabrewulf\sounds\*.*" "..\products\Sabrewulf\sounds\"
copy ".\Sabrewulf\*.hpp" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\*.cpp" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\*.xml" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj" "..\products\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj.filters" "..\products\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj.user" "..\products\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj.filters" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj.user" "..\setups\Sabrewulf\code\"

Echo Copy 3inarow
Echo ----------------------------
copy "..\exe\3inarow.exe" ".\3inarow\"
copy "..\exe\3inarowD.exe" ".\3inarow\"

Echo Copy 3inarow to installers
Echo ----------------------------
copy "..\exe\3inarow.exe" "..\setups\3inarow\"
copy ".\3inarow\resources.icf" "..\setups\3inarow\"

Echo Copy code to produce 3inarow to git
Echo ------------------------
DEL ..\products\3inarow\code\*.hpp
DEL ..\products\3inarow\code\*.cpp
DEL ..\products\3inarow\code\*.xml
DEL ..\products\3inarow\forms\*.* /Q
DEL ..\products\3inarow\boards\*.* /Q
DEL ..\products\3inarow\sounds\*.* /Q
DEL ..\setups\3inarow\code\*.hpp
DEL ..\setups\3inarow\code\*.cpp
DEL ..\setups\3inarow\code\*.xml
copy ".\3inarow\*.hpp" "..\products\3InARow\code\"
copy ".\3inarow\*.cpp" "..\products\3InARow\code\"
copy ".\3inarow\*.xml" "..\products\3InARow\code\"
copy ".\3inarow\forms\*.*" "..\products\3inarow\forms\"
copy ".\3inarow\boards\*.*" "..\products\3inarow\boards\"
copy ".\3inarow\sounds\*.*" "..\products\3inarow\sounds\"
copy ".\3inarow\*.hpp" "..\setups\3InARow\code\"
copy ".\3inarow\*.cpp" "..\setups\3InARow\code\"
copy ".\3inarow\*.xml" "..\setups\3InARow\code\"
copy ".\3inarow\3inarow.vcxproj" "..\products\3InARow\code\"
copy ".\3inarow\3inarow.vcxproj.filters" "..\products\3InARow\code\"
copy ".\3inarow\3inarow.vcxproj.user" "..\products\3InARow\code\"
copy ".\3inarow\3inarow.vcxproj" "..\setups\3InARow\code\"
copy ".\3inarow\3inarow.vcxproj.filters" "..\setups\3InARow\code\"
copy ".\3inarow\3inarow.vcxproj.user" "..\setups\3InARow\code\"

Echo Copy DragonWind II
Echo ------------------------
copy "..\exe\DragonWindII.exe" ".\DragonWindII\"
copy "..\exe\DragonWindIID.exe" ".\DragonWindII\"

Echo Copy DragonWind II The Revenge to installer
Echo ------------------------
copy "..\exe\DragonWindII.exe" "..\setups\DragonWindII\"
copy ".\DragonWindII\resources1.icf" "..\setups\DragonWindII\"
copy ".\DragonWindII\resources2.icf" "..\setups\DragonWindII\"
copy ".\DragonWindII\resources3.icf" "..\setups\DragonWindII\"
copy ".\DragonWindII\resources4.icf" "..\setups\DragonWindII\"
copy ".\DragonWindII\resources5.icf" "..\setups\DragonWindII\"
copy ".\DragonWindII\resources6.icf" "..\setups\DragonWindII\"
copy ".\DragonWindII\gamestates.xml" "..\setups\DragonWindII\"
copy ".\DragonWindII\Instructions.docx" "..\setups\DragonWindII\"

Echo Copy code to produce DragonWind II to git
Echo ------------------------
DEL ..\products\DragonWindII\code\*.hpp
DEL ..\products\DragonWindII\code\*.cpp
DEL ..\products\DragonWindII\code\*.xml
DEL ..\products\DragonWindII\forms\*.* /Q
DEL ..\products\DragonWindII\maps\*.* /Q
DEL ..\products\DragonWindII\sounds\*.* /Q
DEL ..\setups\DragonWindII\code\*.hpp
DEL ..\setups\DragonWindII\code\*.cpp
DEL ..\setups\DragonWindII\code\*.xml
copy ".\DragonWindII\*.hpp" "..\products\DragonWindII\code\"
copy ".\DragonWindII\*.cpp" "..\products\DragonWindII\code\"
copy ".\DragonWindII\*.xml" "..\products\DragonWindII\code\"
copy ".\DragonWindII\forms\*.*" "..\products\DragonWindII\forms\"
copy ".\DragonWindII\maps\*.*" "..\products\DragonWindII\maps\"
copy ".\DragonWindII\sounds\*.*" "..\products\DragonWindII\sounds\"
copy ".\DragonWindII\*.hpp" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\*.cpp" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\*.xml" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\DragonWindII.vcxproj" "..\products\DragonWindII\code\"
copy ".\DragonWindII\DragonWindII.vcxproj.filters" "..\products\DragonWindII\code\"
copy ".\DragonWindII\DragonWindII.vcxproj.user" "..\products\DragonWindII\code\"
copy ".\DragonWindII\DragonWindII.vcxproj" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\DragonWindII.vcxproj.filters" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\DragonWindII.vcxproj.user" "..\setups\DragonWindII\code\"

Echo Copy Battleship II
Echo ------------------------
copy "..\exe\BattleshipII.exe" ".\BattleshipII\"
copy "..\exe\BattleshipIID.exe" ".\BattleshipII\"

Echo Copy Battleship II Strikes back to installer
Echo ------------------------
copy "..\exe\BattleshipII.exe" "..\setups\BattleshipII\"
copy ".\BattleshipII\resources1.icf" "..\setups\BattleshipII\"
copy ".\BattleshipII\resources2.icf" "..\setups\BattleshipII\"
copy ".\BattleshipII\resources3.icf" "..\setups\BattleshipII\"
copy ".\BattleshipII\resources4.icf" "..\setups\BattleshipII\"
copy ".\BattleshipII\resources5.icf" "..\setups\BattleshipII\"
copy ".\BattleshipII\gamestates.xml" "..\setups\BattleshipII\"
copy ".\BattleshipII\Instructions.docx" "..\setups\BattleshipII\"

Echo Copy code to produce Battleship II strickes back to git
Echo ------------------------
DEL ..\products\BattleshipII\code\*.hpp
DEL ..\products\BattleshipII\code\*.cpp
DEL ..\products\BattleshipII\code\*.xml
DEL ..\products\BattleshipII\forms\*.* /Q
DEL ..\products\BattleshipII\maps\*.* /Q
DEL ..\products\BattleshipII\sounds\*.* /Q
DEL ..\setups\BattleshipII\code\*.hpp
DEL ..\setups\BattleshipII\code\*.cpp
DEL ..\setups\BattleshipII\code\*.xml
copy ".\BattleshipII\*.hpp" "..\products\BattleshipII\code\"
copy ".\BattleshipII\*.cpp" "..\products\BattleshipII\code\"
copy ".\BattleshipII\*.xml" "..\products\BattleshipII\code\"
copy ".\BattleshipII\forms\*.*" "..\products\BattleshipII\forms\"
copy ".\BattleshipII\maps\*.*" "..\products\BattleshipII\maps\"
copy ".\BattleshipII\sounds\*.*" "..\products\BattleshipII\sounds\"
copy ".\BattleshipII\*.hpp" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\*.cpp" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\*.xml" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\BattleshipII.vcxproj" "..\products\BattleshipII\code\"
copy ".\BattleshipII\BattleshipII.vcxproj.filters" "..\products\BattleshipII\code\"
copy ".\BattleshipII\BattleshipII.vcxproj.user" "..\products\BattleshipII\code\"
copy ".\BattleshipII\BattleshipII.vcxproj" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\BattleshipII.vcxproj.filters" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\BattleshipII.vcxproj.user" "..\setups\BattleshipII\code\"

Echo Copy PacManII
Echo ------------------------
copy "..\exe\PacManII.exe" ".\PacManII\"
copy "..\exe\PacManIID.exe" ".\PacManII\"

Echo Copy PacManII Strikes back to installer
Echo ------------------------
copy "..\exe\PacManII.exe" "..\setups\PacManII\"
copy ".\PacManII\adresources.icf" "..\setups\PacManII\"
copy ".\PacManII\pcpacman.icf" "..\setups\PacManII\"
copy ".\PacManII\pacmanII.icf" "..\setups\PacManII\"
copy ".\PacManII\adguisystem.xml" "..\setups\PacManII\"
copy ".\PacManII\pcguisystem.xml" "..\setups\PacManII\"

Echo Copy code to produce PacmanII to git
Echo ------------------------
DEL ..\products\PacManII\code\*.hpp
DEL ..\products\PacManII\code\*.cpp
DEL ..\products\PacManII\code\*.xml
DEL ..\products\PacManII\forms\*.* /Q
DEL ..\products\PacManII\maps\*.* /Q
DEL ..\products\PacManII\sounds\*.* /Q
DEL ..\setups\PacManII\code\*.hpp
DEL ..\setups\PacManII\code\*.cpp
DEL ..\setups\PacManII\code\*.xml
copy ".\PacManII\*.hpp" "..\products\PacManII\code\"
copy ".\PacManII\*.cpp" "..\products\PacManII\code\"
copy ".\PacManII\*.xml" "..\products\PacManII\code\"
copy ".\PacManII\forms\*.*" "..\products\PacManII\forms\"
copy ".\PacManII\maps\*.*" "..\products\PacManII\maps\"
copy ".\PacManII\sounds\*.*" "..\products\PacManII\sounds\"
copy ".\PacManII\*.hpp" "..\setups\PacManII\code\"
copy ".\PacManII\*.cpp" "..\setups\PacManII\code\"
copy ".\PacManII\*.xml" "..\setups\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj" "..\products\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj.filters" "..\products\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj.user" "..\products\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj" "..\setups\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj.filters" "..\setups\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj.user" "..\setups\PacManII\code\"
