AR := ar
CC := gcc
CP := cp
MKDIR := mkdir -p
RM := rm

SRC_DIR := src
EX_SRC_DIR := $(SRC_DIR)/examples
EX_FILES := $(shell find $(EX_SRC_DIR) -name *.c)
LIB_SRC_DIR := $(SRC_DIR)/library
LIB_C_FILES := $(shell find $(LIB_SRC_DIR) -name *.c)
LIB_H_FILES := $(shell find $(LIB_SRC_DIR) -name *.h)
BLD_DIR := bld
EX_BLD_DIR := $(BLD_DIR)/examples
EX_TARGETS := $(EX_FILES:$(EX_SRC_DIR)/%.c=$(EX_BLD_DIR)/%.out)
LIB_BLD_DIR := $(BLD_DIR)/library
LIB_DEP_DIR := $(LIB_BLD_DIR)/dependencies
LIB_INC_DIR := $(LIB_BLD_DIR)/include
LIB_OBJ_DIR := $(LIB_BLD_DIR)/objects
LIB_NAME := cspsolve
LIB_TARGET := $(LIB_BLD_DIR)/lib$(LIB_NAME).a
LIB_DEP_TARGETS := $(LIB_C_FILES:$(LIB_SRC_DIR)/%.c=$(LIB_DEP_DIR)/%.d)
LIB_INC_TARGETS := $(LIB_H_FILES:$(LIB_SRC_DIR)/%.h=$(LIB_INC_DIR)/%.h)
LIB_OBJ_TARGETS := $(LIB_C_FILES:$(LIB_SRC_DIR)/%.c=$(LIB_OBJ_DIR)/%.o)

.PHONY : library objects dependencies headers examples style clean

library : $(LIB_TARGET)

$(LIB_TARGET) : I = $(addprefix -I,$(dir $<))
$(LIB_TARGET) : $(LIB_OBJ_TARGETS) $(LIB_DEP_TARGETS) $(LIB_INC_TARGETS)
	$(MKDIR) $(dir $(LIB_TARGET))
	$(AR) -crs $(LIB_TARGET) $(LIB_OBJ_TARGETS)

objects : $(LIB_OBJ_TARGETS)

$(LIB_OBJ_DIR)/%.o : $(LIB_SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(I) -c $< -o $@

dependencies : $(LIB_DEP_TARGETS)

$(LIB_DEP_DIR)/%.d : O_FILE = $(<:$(LIB_SRC_DIR)/%.c=$(LIB_OBJ_DIR)/%.o)
$(LIB_DEP_DIR)/%.d : $(LIB_SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(I) -MM -MQ $(O_FILE) -MF $@ $<

headers : $(LIB_INC_TARGETS)

$(LIB_INC_DIR)/%.h : $(LIB_SRC_DIR)/%.h
	$(MKDIR) $(dir $@)
	$(CP) $< $@

examples : $(EX_TARGETS)

$(EX_BLD_DIR)/%.out : $(EX_SRC_DIR)/%.c headers library
	$(MKDIR) $(dir $@)
	$(CC) -I$(LIB_INC_DIR) -L$(LIB_BLD_DIR) -l$(LIB_NAME) $< -o $@

style :
	clang-format --style=file -i $(SRC_DIR)/**

clean :
	$(RM) -r $(BLD_DIR)

-include $(LIB_DEP_TARGETS)
