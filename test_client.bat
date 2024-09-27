@echo off
cd build
cd client
rm MTR_CLIENT
python make.py test
cd ..
cd ..