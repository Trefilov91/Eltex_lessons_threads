

CC = gcc
AR = ar
AR_FLAGS = rc
BUILD_DIR = build

EXE_INC  = incriment
EXE_SHOP = shop
EXE_SHOP_KEY = shop_key

SHOP_LIB = shop_lib

SRC_INC = \
incriment.c

SRC_SHOP = \
shop.c

SRC_SHOP_KEY = \
shop_key.c\

SRC_SHOP_KEY_LIB = \
shop_key_lib.c

CINC = \
-Iinc

LIB = \
-lpthread

LIBS = \
$(LIB)\
-l$(SHOP_LIB)

all: $(BUILD_DIR) $(EXE_INC) $(EXE_SHOP) $(EXE_SHOP_KEY)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(EXE_INC): $(SRC_INC)
	$(CC) $^ $(LIB) -o $(BUILD_DIR)/$@

$(EXE_SHOP): $(SRC_SHOP)
	$(CC) $^ $(LIB) -o $(BUILD_DIR)/$@

$(EXE_SHOP_KEY): $(SRC_SHOP_KEY) lib$(SHOP_LIB).a
	$(CC) $(CINC) $< -L$(BUILD_DIR) $(LIBS) -o $(BUILD_DIR)/$@

lib$(SHOP_LIB).a: $(SRC_SHOP_KEY_LIB)
	$(CC) -c $(CINC) $^ -o $(BUILD_DIR)/$(SHOP_LIB).o
	$(AR) $(AR_FLAGS) $(BUILD_DIR)/$@ $(BUILD_DIR)/$(SHOP_LIB).o

clean:
	rm -rf $(BUILD_DIR)

