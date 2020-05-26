#Makefile
#Projekt: Camera-Link
#Autor: Adam Bieniek
#Data utworzenia: 25.05.2020
CC := g++
CPPFLAGS := -Iinclude
CFLAGS := -std=c++11 -g -Wall -DBOOST_TEST_DYN_LINK -lboost_unit_test_framework
LDFLAGS := -pthread

SRC_DIR := src
OBJ_DIR := obj
TEST_DIR := tests

EXE := camera gate
TESTS := $(TEST_DIR)/messageTest $(TEST_DIR)/fileMessageTest $(TEST_DIR)/CameraTest
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(EXE)

test: $(TESTS) $(TEST_DIR)/cameraTesting1 #$(TEST_DIR)/gateTesting1 $(TEST_DIR)/multipleCameraTesting

camera: $(OBJ_DIR)/message.o $(OBJ_DIR)/fileMessage.o $(OBJ_DIR)/camera.o
		$(CC) $(LDFLAGS) $^ -o $@

gate: $(OBJ_DIR)/message.o $(OBJ_DIR)/fileMessage.o $(OBJ_DIR)/gate.o
		$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
		$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
		mkdir -p $@

$(TEST_DIR)/messageTest:  $(OBJ_DIR)/messageTest.o $(OBJ_DIR)/message.o | $(TEST_DIR)
		$(CC) $(LDFLAGS) $^ -o $(TEST_DIR)/messageUnitTest

$(TEST_DIR)/fileMessageTest: $(OBJ_DIR)/fileMessageTest.o $(OBJ_DIR)/fileMessage.o| $(TEST_DIR)
		$(CC) $(LDFLAGS) $^ -o $(TEST_DIR)/fileMessageUnitTest

$(TEST_DIR)/CameraTest: $(OBJ_DIR)/CameraTests.o $(OBJ_DIR)/fileMessage.o | $(TEST_DIR)
		$(CC) $(LDFLAGS) $^ -o $(TEST_DIR)/cameraUnitTest

$(TEST_DIR)/cameraTesting1: MKDIRS $(OBJ_DIR)/camera1.o $(OBJ_DIR)/gate1.o
		$(CC) $(LDFLAGS) $(OBJ_DIR)/camera1.o -o $@/camera/camera1
		$(CC) $(LDFLAGS) $(OBJ_DIR)/gate1.o -o $@/gate/gate1

MKDIRS:
		mkdir -p $(TEST_DIR)/cameraTesting1/camera
		mkdir -p $(TEST_DIR)/cameraTesting1/gate
		mkdir -p $(TEST_DIR)/gateTesting1/camera
		mkdir -p $(TEST_DIR)/gateTesting1/gate
		mkdir -p $(TEST_DIR)/multipleCameraTesting/camera
		mkdir -p $(TEST_DIR)/multipleCameraTesting/gate


$(TEST_DIR)/gateTesting1: MKDIRS $(OBJ_DIR)/camera2.o $(OBJ_DIR)/gate2.o #TODO test zaniku napiecia na bramce
		$(CC) $(LDFLAGS) $(OBJ_DIR)/camera2.o -o $@/camera/camera2
		$(CC) $(LDFLAGS) $(OBJ_DIR)/gate2.o -o $@/gate/gate2

$(TEST_DIR)/multipleCameraTesting: MKDIRS $(OBJ_DIR)/camera3.o $(OBJ_DIR)/gate3.o #TODO test wielu kamer
		$(CC) $(LDFLAGS) $(OBJ_DIR)/camera3.o -o $@/camera/camera3
		$(CC) $(LDFLAGS) $(OBJ_DIR)/gate3.o -o $@/gate/gate3

$(TEST_DIR):
		mkdir -p $@

cleanTestTexts:
		rm -f $(TEST_DIR)/*.txt
		rm -f $(TEST_DIR)/gateTesting1/camera/*.txt
		rm -f $(TEST_DIR)/gateTesting1/gate/*.txt
		rm -f $(TEST_DIR)/cameraTesting1/camera/*.txt
		rm -f $(TEST_DIR)/cameraTesting1/gate/*.txt
		rm -f $(TEST_DIR)/multipleCameraTesting/camera/*.txt
		rm -f $(TEST_DIR)/multipleCameraTesting/gate/*.txt

cleanTest: cleanTestTexts
		rm -f $(TESTS)
		rm -f $(TEST_DIR)/gateTesting1/camera/camera2
		rm -f $(TEST_DIR)/gateTesting1/gate/gate2
		rm -f $(TEST_DIR)/cameraTesting1/camera/camera1
		rm -f $(TEST_DIR)/cameraTesting1/gate/gate1
		rm -f $(TEST_DIR)/multipleCameraTesting/camera/camera3
		rm -f $(TEST_DIR)/multipleCameraTesting/gate/gate3

clean: cleanTest
		rm -f $(EXE)
		rm -f $(OBJ_DIR)/*.o