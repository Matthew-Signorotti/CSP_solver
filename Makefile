AR := ar
CC := gcc
CP := cp
MKDIR := mkdir -p
RM := rm

SRC_DIR := src
C_FILES := $(shell find $(SRC_DIR) -name *.c)
H_FILES := $(shell find $(SRC_DIR) -name *.h)
BLD_DIR := bld
DEP_DIR := $(BLD_DIR)/dependencies
INC_DIR := $(BLD_DIR)/include
OBJ_DIR := $(BLD_DIR)/objects
AR_TARGET := $(BLD_DIR)/libcspsolve.a
DEP_TARGETS := $(C_FILES:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)
INC_TARGETS := $(H_FILES:$(SRC_DIR)/%.h=$(INC_DIR)/%.h)
OBJ_TARGETS := $(C_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY : library objects dependencies headers style clean

library : $(AR_TARGET)

$(AR_TARGET) : I = $(addprefix -I,$(dir $<))
$(AR_TARGET) : $(OBJ_TARGETS) $(DEP_TARGETS) $(INC_TARGETS)
	$(MKDIR) $(dir $(AR_TARGET))
	$(AR) -crs $(AR_TARGET) $(OBJ_TARGETS)

objects : $(OBJ_TARGETS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(I) -c $< -o $@

dependencies : $(DEP_TARGETS)

$(DEP_DIR)/%.d : O_FILE = $(<:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
$(DEP_DIR)/%.d : $(SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(I) -MM -MQ $(O_FILE) -MF $@ $<

headers : $(INC_TARGETS)

$(INC_DIR)/%.h : $(SRC_DIR)/%.h
	$(MKDIR) $(dir $@)
	$(CP) $< $@

style :
	clang-format --style=file -i $(SRC_DIR)/**

clean :
	$(RM) -r $(BLD_DIR)

-include $(DEP_TARGETS)
