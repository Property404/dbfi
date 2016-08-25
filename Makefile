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
CFLAGS = -Wall -Wextra -std=gnu99 -O2 -pedantic
FILES = optimize.c interpret.c main.c
SOURCES = $(FILES:%.c=$(SRC_DIR)/%.c)

# Build recipe
bf:
	$(CC) -o $(EXECUTABLE_NAME) $(SOURCES) $(CFLAGS)
clean:
	rm -f $(EXECUTABLE_NAME)
install:
	$(INSTALL_COMMAND) $(EXECUTABLE_NAME) $(INSTALL_DIR)
uninstall:
	rm  $(INSTALL_DIR)/$(EXECUTABLE_NAME)
