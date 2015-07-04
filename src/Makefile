CFLAGS = -Wextra -g -std=c++11
FRAMEWORKS = -framework OpenGL -framework SystemConfiguration -framework Carbon
LIBS = -lglfw3 -lGLEW /Applications/Autodesk/FBX\ SDK/2015.1/lib/clang/debug/libfbxsdk.a `pkg-config --cflags --libs opencv`
INCLUDE = -I../include -I/Applications/Autodesk/FBX\ SDK/2015.1/include
TARGET = ../bin/test
OBJDIR = ./obj
SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

$(TARGET) : $(OBJECTS) 
	g++ $(INCLUDE) $(FRAMEWORKS) $(LIBS) -o $@ $^ -v 

$(OBJDIR)/%.o : %.cpp
	g++ $(CFLAGS) $(INCLUDE) $(FRAMEWORKS) $(LIBS) -o $@ -c $^ -v

all : clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
