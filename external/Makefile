unexport MAKE_DIR CC LD CFLAGS LDFLAGS LIBS INC_SRCH_PATH OBJ_DIR LIB_DIR BIN_DIR BIN

all: lal lclog linih

lal:
	@mkdir -p $(LIB_DIR)
	@$(MAKE) -C $(LIBAL_DIR)
	cp $(LIBAL_DIR)/lib/libal.a $(LIB_DIR)/

lclog:
	@mkdir -p $(LIB_DIR)
	@$(MAKE) -C $(LIBCLOG_DIR) BUILD=""
	cp $(LIBCLOG_DIR)/lib/libclog.a $(LIB_DIR)/
	@$(MAKE) -C $(LIBCLOG_DIR) dev
	cp $(LIBCLOG_DIR)/lib/libclog-dev.a $(LIB_DIR)/

linih:
	@mkdir -p $(LIB_DIR)
	@meson $(LIBINIH_DIR) $(LIBINIH_DIR)/build -D tests=false -D examples=false --default-library static
	@ninja -C $(LIBINIH_DIR)/build
	cp $(LIBINIH_DIR)/build/libinih.a $(LIB_DIR)/
	
.PHONY: clean
clean:
	@$(MAKE) -C $(LIBAL_DIR)   clean
	@$(MAKE) -C $(LIBCLOG_DIR) clean
	@ninja   -C $(LIBINIH_DIR)/build clean
	@echo "Remove Deps"
