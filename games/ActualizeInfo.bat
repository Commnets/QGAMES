@Echo OFF

Echo Copy AticAtac
Echo ------------------------
copy "..\exe\AticAtac.exe" ".\AticAtac\"
copy "..\exe\AticAtacD.exe" ".\AticAtac\"

Echo Copy AticAtac to installer
Echo ------------------------
copy "..\exe\AticAtac.exe" "..\setups\AticAtac\"
copy ".\AticAtac\resources.icf" "..\setups\AticAtac\"

Echo Copy code to produce AticAtac to installer
Echo ------------------------
DEL ..\setups\AticAtac\code\*.hpp
DEL ..\setups\AticAtac\code\*.cpp
DEL ..\setups\AticAtac\code\*.xml
copy ".\AticAtac\*.hpp" "..\setups\AticAtac\code\"
copy ".\AticAtac\*.cpp" "..\setups\AticAtac\code\"
copy ".\AticAtac\*.xml" "..\setups\AticAtac\code\"
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

Echo Copy code to produce Sabrewulf to installer
Echo ------------------------
DEL ..\setups\Sabrewulf\code\*.hpp
DEL ..\setups\Sabrewulf\code\*.cpp
DEL ..\setups\Sabrewulf\code\*.xml
copy ".\Sabrewulf\*.hpp" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\*.cpp" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\*.xml" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj.filters" "..\setups\Sabrewulf\code\"
copy ".\Sabrewulf\Sabrewulf.vcxproj.user" "..\setups\Sabrewulf\code\"

Echo Copy 3inarow
Echo ----------------------------
copy "..\exe\3inarow.exe" ".\3inarow\"
copy "..\exe\3inarowD.exe" ".\3inarow\"

Echo Copy 3inarow to installer
Echo ----------------------------
copy "..\exe\3inarow.exe" "..\setups\3inarow\"
copy ".\3inarow\resources.icf" "..\setups\3inarow\"

Echo Copy code to produce 3inarow to installer
Echo ------------------------
DEL ..\setups\3inarow\code\*.hpp
DEL ..\setups\3inarow\code\*.cpp
DEL ..\setups\3inarow\code\*.xml
copy ".\3inarow\*.hpp" "..\setups\3InARow\code\"
copy ".\3inarow\*.cpp" "..\setups\3InARow\code\"
copy ".\3inarow\*.xml" "..\setups\3InARow\code\"
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

Echo Copy code to produce DragonWind II to installer
Echo ------------------------
DEL ..\setups\DragonWindII\code\*.hpp
DEL ..\setups\DragonWindII\code\*.cpp
DEL ..\setups\DragonWindII\code\*.xml
copy ".\DragonWindII\*.hpp" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\*.cpp" "..\setups\DragonWindII\code\"
copy ".\DragonWindII\*.xml" "..\setups\DragonWindII\code\"
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

Echo Copy code to produce Battleship II strickes back to installer
Echo ------------------------
DEL ..\setups\BattleshipII\code\*.hpp
DEL ..\setups\BattleshipII\code\*.cpp
DEL ..\setups\BattleshipII\code\*.xml
copy ".\BattleshipII\*.hpp" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\*.cpp" "..\setups\BattleshipII\code\"
copy ".\BattleshipII\*.xml" "..\setups\BattleshipII\code\"
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

Echo Copy code to produce PacmanII to installer
Echo ------------------------
DEL ..\setups\PacManII\code\*.hpp
DEL ..\setups\PacManII\code\*.cpp
DEL ..\setups\PacManII\code\*.xml
copy ".\PacManII\*.hpp" "..\setups\PacManII\code\"
copy ".\PacManII\*.cpp" "..\setups\PacManII\code\"
copy ".\PacManII\*.xml" "..\setups\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj" "..\setups\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj.filters" "..\setups\PacManII\code\"
copy ".\PacManII\PacManII.vcxproj.user" "..\setups\PacManII\code\"
