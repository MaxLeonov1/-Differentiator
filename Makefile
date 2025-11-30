CXX := g++

INCLUDES := -I src -I stk_lib -I utils -I log_utils -I input_func

SOURCES := src/main.cpp src/tree.cpp src/diff_sup_func.cpp src/save_load_func.cpp src/diff_func.cpp \
           src/eq_simpl.cpp src/menu.cpp src/errors.cpp \
           log_utils/tree_dump.cpp log_utils/dump_tex.cpp log_utils/logger.cpp \
           utils/sup_func.cpp \
           input_func/input.cpp


OBJ_DIR := obj
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
TARGET := diff.exe

DEBUG_FLAGS := -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
               -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
               -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
               -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
               -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
               -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
               -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
               -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code \
               -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
               -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
               -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging \
               -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla

RELEASE_FLAGS := -O2 -std=c++17

SANITIZER_LDFLAGS := -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr 

all: debug

debug: CXXFLAGS = $(DEBUG_FLAGS) $(INCLUDES)
debug: LDFLAGS = $(SANITIZER_LDFLAGS)
debug: $(TARGET)

release: CXXFLAGS = $(RELEASE_FLAGS) $(INCLUDES) 
release: LDFLAGS =
release: $(TARGET)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	chmod +x $(TARGET)

clean: logclean
	rm -rf $(OBJ_DIR)

logclean:
	rm -rf logs
	rm -f tex_log.tex
