@echo off
setlocal enabledelayedexpansion 
set curdir=%~dp0premake5.lua
cd ../Tool/premake-5.0.0-beta2-windows/
premake5 --file=%curdir% vs2022
pause