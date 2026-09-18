#include "../Include/stdint.h"

typedef enum
{
    KEY_A = 0x80,
    KEY_B = 0x81,
    KEY_C = 0x82
} Keys;

#define INVALID_KEY 0xFF

// EVM = Eletronic Voting Machine
typedef VOID (*EVMCallback)(BYTE key);

static const BYTE key_map[4][4] =
{
    { '1', '2', '3', KEY_A   },
    { '4', '5', '6', KEY_B  },
    { '7', '8', '9', KEY_C },
    { INVALID_KEY, '0', INVALID_KEY, INVALID_KEY }
};

VOID init_evm_keyboard(EVMCallback callback);
VOID tract_key(BYTE line_mask, BYTE column_mask);
