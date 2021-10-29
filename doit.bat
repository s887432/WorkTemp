@echo off

for /f "tokens=*" %%a in (test.txt) do (
	echo %%a
)
pause
