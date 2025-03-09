@echo off

echo Building program...
@REM https://superuser.com/a/806371
set HOME=.
C:\devkitPro\msys2\usr\bin\bash.exe -lc "make clean && make"


echo.
echo Creating release...
if exist "release" (
    rd /s /q "release"
)
mkdir "release"


echo.
echo 'apps' folder...
mkdir .\release\apps\WiiLikeToParty
copy /V .\WiiLikeToParty.dol .\release\apps\WiiLikeToParty\boot.dol
copy /V .\WiiLikeToParty.elf .\release\apps\WiiLikeToParty\alt_for_nerds.elf
copy /V .\assets\hb\* .\release\apps\WiiLikeToParty\*


echo.
echo 'wad' folder...
mkdir .\release\wad
copy /V .\wad\ios\*.wad .\release\wad\*.wad
copy /V .\wad\forwarder_WL2P.wad .\release\wad\forwarder_WL2P.wad


echo.
echo Compressing release...
for /f "tokens=* delims=" %%a in ('git describe --tags --always') do set GIT_VERSION=%%a
echo %GIT_VERSION%

if exist .\WiiLikeToParty_%GIT_VERSION%.zip (
    del /q .\WiiLikeToParty_%GIT_VERSION%.zip
)
powershell -c "Compress-Archive -CompressionLevel Optimal -Path '.\release\*' -DestinationPath '.\WiiLikeToParty_%GIT_VERSION%.zip'"

echo.
echo Done!
