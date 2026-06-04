
clang-tidy.exe -header-filter='./src' -extra-arg=-std=c++20 src/contour.cpp
clang-tidy.exe -header-filter='./src' -extra-arg=-std=c++20 src/contourtools.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/geo.cpp
clang-tidy.exe -header-filter='./src' -extra-arg=-std=c++20 src/glmesh3d.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/glviewwidget.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/mesh3d.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/mesh3dbuilder.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/mesh3dsaver.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/meshclip.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/meshtools.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/meshtrianglebuilder.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/plane3d.cpp
clang-tidy.exe -extra-arg=-std=c++20 src/colorgenerator.cpp
rem clang-tidy.exe -extra-arg=-std=c++20 src/testclip.cpp
rem clang-tidy.exe -extra-arg=-std=c++20 src/main.cpp
