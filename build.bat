@echo off
IF NOT EXIST build mkdir build
pushd build
:: GENERAL compilerFlags FLAGS
set compilerFlags=					-MTd	&:: Include CRT library in the executable (static link) (d uses debug CRT)
set compilerFlags=%compilerFlags%	-nologo &:: Suppress compiler info
set compilerFlags=%compilerFlags%	-fp:fast	&:: optimized floating pointing calculations
set compilerFlags=%compilerFlags%	-Gm-	&:: Disable minimal rebuild
set compilerFlags=%compilerFlags%	-GR-	&:: Disable runtime type info (C++)
set compilerFlags=%compilerFlags%	-EHa	&:: Disable exception handling (C++)
set compilerFlags=%compilerFlags%	-Od		&:: No optimizations (slow)
set compilerFlags=%compilerFlags%	-Oi		&:: Use assembly intrinsics where possible
set compilerFlags=%compilerFlags%	-WX		&:: Treat all warnings as errors
set compilerFlags=%compilerFlags%	-W4		&:: Display warnings up to level 4
:: IGNORE WARNINGS
set compilerFlags=%compilerFlags%	-wd4201 &:: Nameless struct/union
set compilerFlags=%compilerFlags%	-wd4100 &:: Unused function parameter
set compilerFlags=%compilerFlags%	-wd4189 &:: Local variable not referenced
set compilerFlags=%compilerFlags%	-wd4505	&:: unreferenced function
:: CROSS_PLATFORM DEFINES
set compilerFlags=%compilerFlags%	-DATOMOSZ_INTERNAL=1	&:: 0 - Build for public release 1 - Build for developer only
set compilerFlags=%compilerFlags%	-DATOMOSZ_DEBUG=1
set compilerFlags=%compilerFlags%	-DATOMOSZ_NOTIMPLEMNTEDYET=0
:: DEBUG VARIABLES
set compilerFlags=%compilerFlags%	-FC     &:: Produce the full path of the source code file when printing errors and other messages
set compilerFlags=%compilerFlags%	-Z7     &:: Produce debug information

:: OPTIMIZATIONS
:: set optimizations=					-O2		&:: all optimizations (fast)
:: set optimizations=%optimizations%	-Oi		&:: all optimizations (fast)

:: COMMON LINKER SWITCHES       		
set commonLinkerFlags= -incremental:no
set commonLinkerFlags=%commonLinkerFlags% -opt:ref	&:: Remove unused functions

:: Windows build version
set system=					-subsystem:windows,10.0 &:: Windows 10 & 11 = 10.0, Windows 8 = 6.2

:: DLL LINKER SWITCHES
set dll_link=/EXPORT:GameUpdateAndRender /EXPORT:GameInitialize

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
cl %compilerFlags% ..\Source\AtomosZEngine\AtomosZEngine.cpp			-FmAtomosZEngine.map		-LD	/link %commonLinkerFlags% -PDB:AtomosZ_%random%.pdb %dll_link%
del lock.tmp


set commonLinkerFlags=%commonLinkerFlags% user32.lib gdi32.lib winmm.lib	&:: WIN32 PLATFORM LIBRARIES
set commonLinkerFlags=%commonLinkerFlags% D3D11.lib DXGI.lib				&:: DIRECTX LIBRARIES
set directXTK=..\packages\DirectXTK-feb2023\Bin\Desktop_2022_Win10\x64\Debug\DirectXTK.lib


cl %compilerFlags% /FeWin32_TenderTorrent.exe ..\Source\AtomosZEngine\Win32_AtomosZ.cpp	-Fmwin32_TenderTorrent.map	/link %commonLinkerFlags% %directXTK% %system%
popd