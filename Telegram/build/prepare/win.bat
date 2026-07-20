@echo OFF

set "FullScriptPath=%~dp0"

REM Keep dependency preparation within hosted-runner resource limits.
if not defined TZ_PREPARE_THREADS set "TZ_PREPARE_THREADS=2"
set "NUMBER_OF_PROCESSORS=%TZ_PREPARE_THREADS%"
if not defined CL_MPCount set "CL_MPCount=1"
if not defined UseMultiToolTask set "UseMultiToolTask=false"
if not defined MSBUILDDISABLENODEREUSE set "MSBUILDDISABLENODEREUSE=1"

python "%FullScriptPath%prepare.py" %*
if %errorlevel% neq 0 goto error

exit /b

:error
echo FAILED
exit /b 1
