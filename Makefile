SHELL = /bin/sh

# Where the files are
SRC_DIR = ./src/

# Where to install
# For Windows/ReactOS:
#	Change INSTALL_DIR to C:\windows\sytem32\
#	Change INSTALL_COMMAND to move
#	Change EXECUTABLE_NAME to bf.exe
INSTALL_DIR = /usr/local/bin/
INSTALL_COMMAND = install
EXECUTABLE_NAME = bf

# Compiler flags
CC = cc # More portable than using 'gcc,' but with risk
	# of calling wrong compiler
CFLAGS = -Wall -Wextra -std=gnu99 -O3 -s -pedantic
FILES = interpret.c main.c optimize.c
OBJECTS = $(FILES:%.c=$(SRC_DIR)/%.o)

# Build recipe
bf: $(OBJECTS)
	$(CC) -o $(EXECUTABLE_NAME) $(OBJECTS) $(CFLAGS)
%.o: %.c
	$(CC) $(CCFLAGS) -c $(@:.o=.c) -o $@
clean:
	rm -f $(EXECUTABLE_NAME)
	rm -f $(OBJECTS)
install:
	$(INSTALL_COMMAND) $(EXECUTABLE_NAME) $(INSTALL_DIR)
uninstall:
	rm  $(INSTALL_DIR)/$(EXECUTABLE_NAME)
