cd %~dp0..
msbuild %~dp0..\ApplicationUpdateService.sln /p:Configuration=Release
del /f /q %~dp0..\build\*
ISCC %~dp0..\pkg\update-service.iss
