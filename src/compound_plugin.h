#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Name of the plugin.
#define PLUGIN_NAME "Compound"

// TODO: add doc.
#define NUM_SELECTORS        11
#define TOKEN_SENT_FOUND     1
#define TOKEN_RECEIVED_FOUND (1 << 1)

typedef enum {
    COMPOUND_MINT = 0,
    COMPOUND_REDEEM,
    COMPOUND_REDEEM_UNDERLYING,
    COMPOUND_BORROW,
    COMPOUND_REPAY_BORROW,
    COMPOUND_REPAY_BORROW_ON_BEHALF,
    COMPOUND_TRANSFER,
    COMPOUND_LIQUIDATE_BORROW,
    COMPOUND_MANUAL_VOTE,
    COMPOUND_VOTE_DELEGATE
} compoundSelector_t;

typedef enum {
    MINT_AMOUNT = 0,
    REDEEM_TOKENS,
    REDEEM_AMOUNT,
    RECIPIENT,
    BORROW_AMOUNT,
    BORROWER,
    AMOUNT,
    REPAY_AMOUNT,
    DELEGATEE,
    COLLATERAL,
    PROPOSAL_ID,
    SUPPORT,
    UNEXPECTED_PARAMETER
} parameter;
typedef struct compoundAssetDefinition_t {
    char ticker[MAX_TICKER_LEN];
    uint8_t decimals;
} compoundAssetDefinition_t;

typedef struct context_t {
    // For display.
    uint8_t dest[ADDRESS_LENGTH];
    uint8_t collateral[ADDRESS_LENGTH];
    uint8_t amount[INT256_LENGTH];
    uint8_t proposal_id[INT256_LENGTH];
    uint8_t support[INT256_LENGTH];

    char ticker[MAX_TICKER_LEN];
    uint8_t decimals;
    uint8_t token_found;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.

    // For both parsing and display.
    compoundSelector_t selectorIndex;
} context_t;

extern const uint8_t *const COMPOUND_SELECTORS[NUM_SELECTORS];

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);