# コンパイラとオプション
CXX = g++
CXXFLAGS = -std=c++11 $(shell root-config --cflags)
LDFLAGS = $(shell root-config --glibs)

# ソースファイルとオブジェクトファイル
SOURCES = src/GUImain.cpp src/GUI_Dict.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# 実行ファイル
EXECUTABLE = MyApp

# デフォルトターゲット
all: $(EXECUTABLE)

# 実行ファイルのビルド
$(EXECUTABLE): $(OBJECTS)
    $(CXX) -o $@ $^ $(LDFLAGS)

# オブジェクトファイルのビルド
%.o: %.cpp
    $(CXX) -c -o $@ $< $(CXXFLAGS)

# rootclingで辞書ファイルを生成
src/GUI_Dict.cpp: include/GUImain.hpp include/analysis.hpp include/LinkDef.hpp
    rootcling -f $@ -c $^

# クリーンアップ
clean:
    rm -f $(OBJECTS) $(EXECUTABLE) src/GUI_Dict.cpp

.PHONY: all clean
