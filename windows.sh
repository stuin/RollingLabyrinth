files="src/main.cpp src/Skyrmion/GridMaker.cpp src/Skyrmion/Node.cpp src/Skyrmion/UpdateList.cpp"
options="-pthread -O3 -std=c++17 -Wall -fpermissive -mwindows -static -DSFML_STATIC"
libs="-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lwinmm -lgdi32 -lopengl32"

output="RollingLabyrinth"

cmd="x86_64-w64-mingw32-g++ $files -o $output.exe $options $libs"
cp /tmp/a.out "$output.out"

echo $cmd
$cmd

echo "Done, Creating zip"
zip -r RollingLabyrinth.zip RollingLabyrinth.* res/

