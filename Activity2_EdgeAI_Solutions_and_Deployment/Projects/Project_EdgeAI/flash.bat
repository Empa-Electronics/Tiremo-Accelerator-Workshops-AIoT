@echo off
if exist build rd /s /q build
cmake --preset debug
cmake --build --preset debug
cmake --build --preset flash-debug
