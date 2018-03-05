pushd ..\source
call compile.bat
popd
pngtopnm canProfile.png > can.pbm
pbmgetProfile can.pbm > can.csv
