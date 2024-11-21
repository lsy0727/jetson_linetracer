CX = g++
CXFLAGS = -g -Wall
CVFLAGS = `pkg-config opencv4 --cflags --libs`
# 백쿼트(`명령어`)는 쉘에서 명령어를 실행한 결과를 의미

TARGET = vision
OBJS = main.o vision.o

$(TARGET) : $(OBJS)
	$(CX) -o $(TARGET) $(OBJS) $(CVFLAGS)
main.o : main.cpp vision.hpp
	$(CX) $(CXFLAGS) -c main.cpp $(CVFLAGS)
vision.o : vision.cpp vision.hpp
	$(CX) $(CXFLAGS) -c vision.cpp $(CVFLAGS)

.PHONY: clean

clean:
	rm -rf $(TARGET) $(OBJS)