# **************************************************************************** #
#                                    CONFIG                                    #
# **************************************************************************** #

NAME        := ircserv

CXX         := c++
CXXFLAGS    := -Wall -Werror -Wextra -std=c++98
CPPFLAGS    := -MMD -MP -I include

SRC_DIR     := src
BUILD_DIR   := build

# Recursively collect all .cpp files
SRCS        := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS        := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

# **************************************************************************** #
#                                 UX SETTINGS                                  #
# **************************************************************************** #

# Verbosity: default quiet (pretty logs only). Use `make V=1` to show commands
ifeq ($(V),1)
Q :=
else
Q := @
endif

# Colors (always on)
CLR_RESET  := \033[0m
CLR_BOLD   := \033[1m
CLR_RED    := \033[31m
CLR_GRN    := \033[32m
CLR_YLW    := \033[33m
CLR_MAG    := \033[35m
CLR_CYN    := \033[36m
OK_SYM     := ✓
ERR_SYM    := ✗

TAG_CC     := $(CLR_CYN)[CC]$(CLR_RESET)
TAG_LD     := $(CLR_MAG)[LD]$(CLR_RESET)
TAG_CLEAN  := $(CLR_YLW)[CLEAN]$(CLR_RESET)
TAG_FCLEAN := $(CLR_YLW)[FCLEAN]$(CLR_RESET)

# **************************************************************************** #
#                                     RULES                                    #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(TAG_LD) Linking $(CLR_BOLD)%s$(CLR_RESET)\n" "$@"
	$(Q)$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@printf "$(CLR_GRN)$(OK_SYM) Built $(CLR_BOLD)%s$(CLR_RESET)\n" "$@"

# Compile each .cpp -> build/.o (create folders as needed) with nice logs
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@printf "$(TAG_CC) %s\n" "$<"
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@printf "$(TAG_CLEAN) removing object files in %s\n" "$(BUILD_DIR)/"
	$(Q)rm -rf $(BUILD_DIR)

fclean: clean
	@printf "$(TAG_FCLEAN) removing binary %s\n" "$(NAME)"
	$(Q)rm -f $(NAME)

re: fclean all

# Convenience extras (not used by default mandatory build)
debug: CXXFLAGS += -g3 -O0
debug: re

asan: CXXFLAGS += -g3 -O1 -fno-omit-frame-pointer -fsanitize=address
asan: re

-include $(DEPS)

.PHONY: all clean fclean re debug asan
