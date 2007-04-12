@if "%1"=="" goto showhelp
@if exist kzip_temp_dir goto endit
mkdir kzip_temp_dir
pkzip25 -ext -dir %1 kzip_temp_dir
cd kzip_temp_dir
@if exist kzip_temp_zip goto skipit
..\kzip /rn /r /y /b128 /rn kzip_temp_zip * %2 %3 %4 %5
@if not errorlevel 0 goto skipit
..\zipmix ..\%1 kzip_temp_zip.zip
:skipit
cd..
rd /s /q kzip_temp_dir
@goto endit
:showhelp
@echo Usage: rekzip [.ZIP filename] (KZIP options)
@echo    (requires KZIP.EXE, ZIPMIX.EXE, and PKZIP25.EXE to be in path)
:endit
pause
