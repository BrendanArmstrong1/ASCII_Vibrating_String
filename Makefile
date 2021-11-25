
CC = gcc
CFLAGS += -Wall -g -lm
DEPTH = 2
SRCS := $(shell find . -maxdepth $(DEPTH) -name '*.c' | sed 's/.*\///g')

OBJDIR := .build
OBJ := $(SRCS:%.c=$(OBJDIR)/%.o)

DEPDIR = .deps
DEPFILES := $(SRCS:%.c=$(DEPDIR)/%.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

.PHONY: clean
TARGET := Output


$(OBJDIR)/%.o: | $(OBJDIR) $(DEPDIR)
	@$(CC) -c $(CFLAGS) $(DEPFLAGS) -o $@ $(shell find . -maxdepth $(DEPTH) -name $(patsubst %.o, %.c,$(@F)))

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -rf $(OBJDIR) $(DEPDIR) $(TARGET)

$(OBJDIR) $(DEPDIR):
	@mkdir -p $@

$(DEPFILES):

include $(wildcard $(DEPFILES))
