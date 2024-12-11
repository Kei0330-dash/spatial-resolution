cmake_minimum_required(VERSION 3.10)

# プロジェクト名とバージョン
project(MyApp VERSION 1.0)

# ROOTの設定
find_package(ROOT REQUIRED)

# インクルードディレクトリを指定
include_directories(${ROOT_INCLUDE_DIRS})

# ヘッダーファイルを指定してrootclingを実行
execute_process(COMMAND rootcling -f ${CMAKE_SOURCE_DIR}/src/GUI_Dict.cpp -c ${CMAKE_SOURCE_DIR}/include/GUImain.hpp ${CMAKE_SOURCE_DIR}/include/analysis.hpp ${CMAKE_SOURCE_DIR}/include/LinkDef.hpp)

# ソースファイル
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/GUImain.cpp
    ${CMAKE_SOURCE_DIR}/src/GUI_Dict.cpp
)

# 実行可能ファイルを生成
add_executable(MyApp ${SOURCES})

# ROOTライブラリをリンク
target_link_libraries(MyApp ${ROOT_LIBRARIES})
