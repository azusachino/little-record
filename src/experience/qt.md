# QT

## build a project

1. qt creator configure the project
2. switch to build mode, better clean, then run build (if changed the `.pro` file, need to run qmake)
3. build the release
4. run `qtwindows.exe` to extract the `app.exe`
5. copy every necessary `dll` file to the relase folder
6. make sure qt related resources files are accessible by the `app.exe`

## qt 1.5.12 bug

编译时，可能不会一并复制 resources 文件夹，需要手动复制缺失的 dll 文件。
