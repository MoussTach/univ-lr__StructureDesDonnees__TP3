EXEC=AVLtree
DEBUG=y

####################

CXX=gcc

CXXFLAGS=-ansi -std=c11
ifeq ($(DEBUG),y)
	CXXFLAGS+=-g -g3
else
	CXXFLAGS+= -Os
endif

LDFLAGS=
LIBS=
OBJDIR=./build
SOURCE_DIRECTORY=

SRC=$(shell find src -type f -iname '*.c')
OBJ=$(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(OBJDIR) $(EXEC) clean

$(OBJDIR):
	mkdir -p $(OBJDIR) $(addprefix $(OBJDIR)/, $(shell find src -type d))

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJDIR)

mrproper: clean
	rm -rf $(EXEC)

