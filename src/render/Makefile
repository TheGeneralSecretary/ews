SRC_DIR = .
LIB  = $(LIB_DIR)/librender.a
SRCS = $(wildcard *.c)
_OBJ = $(SRCS:.c=.o)
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	@$(AR) cr $@ $^
	@echo "    Archive    $(notdir $@)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)
	@echo "    CC     $(OBJS)"

.PHONY: clean
clean:
	@$(RM) -f $(LIB) $(OBJS)
	@$(RM) -f *.expand
	@echo "    Remove Objects:   $(OBJS)"
