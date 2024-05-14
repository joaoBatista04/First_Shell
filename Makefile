EXECUTABLE       = fsh
SRC              = ./src
INCLUDE          = ./include
OBJ              = ./obj
FLAGS            = -lm -pedantic -Wall -g
ADD_FLAGS        = -Wextra -O2
COMPILER         = gcc   

# Path and extesions
C_FILES        = $(wildcard $(SRC)/*.c | $(SRC)/**/*.c)
OBJ_PATH_FILES = $(patsubst $(SRC)%,$(OBJ)%,$(C_FILES))
OBJ_FILES      = $(patsubst %.c,%.o,$(OBJ_PATH_FILES))

# Colors
YELLOW = "\033[1;33m"
RED = "\033[1;31m" 
CYAN = "\033[1;36m"
PURPLE = "\033[1;35m"
RESET_COLOR = "\033[0m"

#===========================================================#
# Main target
all: clean create_dir $(OBJ_FILES) $(EXECUTABLE)

# Creatig objects directory
create_dir: 
# @ echo $(PURPLE)
# @ echo "Creating $(OBJ) directory...\n"
	@ mkdir $(OBJ)
# @ echo $(RESET_COLOR)

# Compiling cpp files
$(OBJ)/%.o: $(SRC)/%.c 
# @ echo $(YELLOW)
# @ echo "Compiling file $<..."
	@ $(COMPILER) -c $< -o $@
# @ echo $(RESET_COLOR)

# Creating executable
$(EXECUTABLE): 
# @ echo $(YELLOW)
# @ echo "Creating executable..."
	@ $(COMPILER) $< $(OBJ)/*.o -o $@ $(FLAGS)
# @ echo $(RESET_COLOR)

# Cleaning workspace
clean:
# @ echo $(RED)
# @ echo "Cleaning workspace..."
	@ rm -rf $(OBJ) $(EXECUTABLE)
# @ echo $(RESET_COLOR)

run:
	@ ./fsh