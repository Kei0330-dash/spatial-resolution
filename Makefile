# コンパイラとオプション
CXX = g++
CXXFLAGS = -std=c++11 $(shell root-config --cflags)
LDFLAGS = $(shell root-config --glibs)

# ソースファイルとオブジェクトファイル
SOURCES = src/GUImain.cpp build/GUI_Dict.cpp src/analysis.cpp src/block.cpp src/MyClass.cpp src/mem_root.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# 実行ファイル
EXECUTABLE = MyApp
BUILD_DIR = build

# デフォルトターゲット
all: $(BUILD_DIR)/$(EXECUTABLE)

# 実行ファイルのビルド
$(BUILD_DIR)/$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CXX) -g -o $@ $^ $(LDFLAGS)

# オブジェクトファイルのビルド
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# rootclingで辞書ファイルを生成
$(BUILD_DIR)/GUI_Dict.cpp: include/GUImain.hpp include/analysis.hpp include/LinkDef.hpp include/block.hpp include/alias.hpp include/mem_root.hpp
	rootcling -f $(BUILD_DIR)/GUI_Dict.cpp -c include/GUImain.hpp include/analysis.hpp include/LinkDef.hpp

# クリーンアップ
clean:
	rm -f $(OBJECTS) $(BUILD_DIR)/$(EXECUTABLE) $(BUILD_DIR)/GUI_Dict.cpp
	rmdir $(BUILD_DIR)

.PHONY: all clean
