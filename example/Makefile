CXX := clang++
INCLUDE = -I$(WORKSPACE_FOLDER)/include
CFLAGS = `pkg-config --cflags opencv` -std=c++14
CFLAGS += -O3 # 使用O0优化
CFLAGS += -D LINUX # 添加Linux宏定义
LIBS = `pkg-config --libs opencv` -lpthread -lMVSDK
TARGET = demo
OBJ_DIR = ${WORKSPACE_FOLDER}/obj
LIB_DIR = ${WORKSPACE_FOLDER}/lib

OBJS += $(OBJ_DIR)/camera_manager.o
OBJS += $(OBJ_DIR)/mindvision_camera.o $(OBJ_DIR)/usb_camera.o
OBJS += $(OBJ_DIR)/systime.o

CAMERA_SOURCE = $(WORKSPACE_FOLDER)/src/camera
SOURCE = $(WORKSPACE_FOLDER)/src

all:$(OBJS)
	ar rcs $(LIB_DIR)/libRmCamSDK.a $^ 
	$(CXX) $(CFLAGS) demo.cpp -L$(LIB_DIR) -lRmCamSDK $(INCLUDE) $(LIBS) -o $(TARGET)

$(OBJ_DIR)/camera_manager.o:$(SOURCE)/camera_manager.cpp
	$(CXX) $(CFLAGS) -fPIC -c $^ -o $@ $(INCLUDE) $(LIBS)

$(OBJ_DIR)/mindvision_camera.o:$(CAMERA_SOURCE)/mindvision_camera.cpp
	$(CXX) $(CFLAGS) -fPIC -c $^ -o $@ $(INCLUDE) $(LIBS)

$(OBJ_DIR)/usb_camera.o:$(CAMERA_SOURCE)/usb_camera.cpp
	$(CXX) $(CFLAGS) -fPIC -c $^ -o $@ $(INCLUDE) $(LIBS)

$(OBJ_DIR)/systime.o:$(SOURCE)/systime.cpp
	$(CXX) $(CFLAGS) -fPIC -c $^ -o $@ $(INCLUDE) $(LIBS)


.PHONY:clean

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_DIR)/*
	rm $(TARGET)
