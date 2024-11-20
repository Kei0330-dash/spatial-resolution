# ROOTの設定を取得
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

# コンパイルするファイル
SOURCES      := GUImain.cpp
OBJECTS      := $(SOURCES:.cpp=.o)
EXECUTABLE   := GUImain

# コンパイラの設定
CXX          := g++
CXXFLAGS     := -O2 -Wall $(ROOTCFLAGS)
LDFLAGS      := $(ROOTLIBS)

# ターゲットの規則
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJECTS) $(EXECUTABLE)
