@ECHO OFF
g++ *.cpp
a.exe < test_case_random.txt
start "" "file://%cd:\=/%/documentation/index.html"
plot.py
exit