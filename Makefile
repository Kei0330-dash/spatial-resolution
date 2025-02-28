# コンパイラとオプション
CXX = g++
CXXFLAGS = -g -std=c++11 $(shell root-config --cflags)
LDFLAGS = $(shell root-config --cflags --glibs)

# ディレクトリ
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include

# ソースファイルとオブジェクトファイル
SOURCES = $(SRC_DIR)/GUImain.cpp $(SRC_DIR)/analysis.cpp $(SRC_DIR)/block.cpp \
			$(SRC_DIR)/MyClass.cpp $(SRC_DIR)/mem_root.cpp $(SRC_DIR)/output.cpp $(BUILD_DIR)/GUI_Dict.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# 実行ファイル
EXECUTABLE = $(BUILD_DIR)/MyApp

# デフォルトターゲット
all: $(EXECUTABLE)

# 実行ファイルのビルド
$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# オブジェクトファイルのビルド
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# rootclingで辞書ファイルを生成
$(BUILD_DIR)/GUI_Dict.cpp: $(INC_DIR)/GUImain.hpp $(INC_DIR)/analysis.hpp $(INC_DIR)/block.hpp \
						$(INC_DIR)/alias.hpp $(INC_DIR)/mem_root.hpp $(INC_DIR)/output.hpp $(INC_DIR)/LinkDef.hpp
	mkdir -p $(BUILD_DIR)
	rootcling -f $@ -c $(INC_DIR)/GUImain.hpp $(INC_DIR)/analysis.hpp $(INC_DIR)/block.hpp \
				$(INC_DIR)/alias.hpp $(INC_DIR)/mem_root.hpp $(INC_DIR)/output.hpp $(INC_DIR)/LinkDef.hpp


# クリーンアップ
clean:
	rm -f $(BUILD_DIR)/*.o $(EXECUTABLE) $(BUILD_DIR)/GUI_Dict.cpp
	rmdir $(BUILD_DIR)

.PHONY: all clean
