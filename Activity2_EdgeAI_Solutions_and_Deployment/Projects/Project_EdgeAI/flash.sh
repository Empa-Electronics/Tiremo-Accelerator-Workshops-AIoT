rm -rf build
cmake --preset debug
cmake --build --preset debug

../../../abov-openocd/src/openocd \
  -s Tool/OpenOCD/share/openocd/scripts \
  -f openocd.cfg \
  -c init -c halt \
  -c "program build/debug/tmpl_userapp.hex verify" \
  -c "reset run" -c exit