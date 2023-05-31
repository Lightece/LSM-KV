#
#LINK.o = $(LINK.cc)
#CXXFLAGS = -std=c++14 -Wall
#
#all: correctness persistence
#
#correctness: kvstore.o correctness.o
#
#persistence: kvstore.o persistence.o

# 编译器
CXX = g++
# 编译参数
CXXFLAGS = -std=c++11 -Wall -Wextra

# 目标文件
TARGET = main

# 源文件
SRCS = main.cpp kvstore.cc sstable.cpp

# 生成的目标文件
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 依赖关系
$(OBJS): kvstore.h sstable.h


#
#clean:
#	-rm -f correctness persistence *.o
