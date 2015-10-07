@echo off

:start
  echo.
  pause
  cd C:\Users\David\Documents\Projects\C\RCS
  cls
:compile
  C:\MinGW\bin\gcc.exe main.c list.c token.c char.c parse.c tree.c file.c linear_interpreter.c list.h token.h char.h parse.h tree.h file.h linear_interpreter.h -o rcs
:run
  pause
  rcs test.rcs
  pause
  goto start