@echo off
cmake --preset debug
cmake --build --preset debug
cmake --build --preset flash-debug
