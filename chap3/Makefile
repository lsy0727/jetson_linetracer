CX = g++   
CXFLAGS = -g -Wall
CVFLAGS = `pkg-config opencv4 --cflags --libs`
DXLFLAGS = -I/usr/local/include/dynamixel_sdk -ldxl_x64_cpp

TARGET = linetracer
OBJS = main.o vision.o dxl.o
$(TARGET) : $(OBJS)
	$(CX) -o $(TARGET) $(OBJS) $(CVFLAGS) $(DXLFLAGS)
main.o : main.cpp vision.hpp dxl.hpp
	$(CX) $(CXFLAGS) -c main.cpp $(CVFLAGS) $(DXLFLAGS)
vision.o : vision.hpp vision.cpp
	$(CX) $(CXFLAGS) -c vision.cpp $(CVFLAGS)
dxl.o : dxl.cpp dxl.hpp
	$(CX) $(CXFLAGS) -c dxl.cpp $(DXLFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)