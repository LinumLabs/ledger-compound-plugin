/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "os.h"
#include "cx.h"

#include "compound_plugin.h"

#define NUM_SELECTORS 12
typedef struct compound_parameters_t {
    uint8_t selectorIndex;
    uint8_t amount[32];
    char ticker_1[MAX_TICKER_LEN];
    uint8_t decimals;
} compound_parameters_t;

typedef struct underlying_asset_decimals_t {
    char c_ticker[MAX_TICKER_LEN];
    uint8_t decimals;
} underlying_asset_decimals_t;

/* Sadly, we don't have the information about the underlying asset's decimals, which can differ from
the cToken decimals. Therefore, we hardcode a binding table. If Compound adds a lot of token in the
future, we will have to move to a CAL based architecture instead, as this one doesn't scale well.*/
#define NUM_COMPOUND_BINDINGS 9
const underlying_asset_decimals_t UNDERLYING_ASSET_DECIMALS[NUM_COMPOUND_BINDINGS] = {
    {"cDAI", 18},
    {"CETH", 18},
    {"CUSDC", 6},
    {"CZRX", 18},
    {"CUSDT", 6},
    {"CBTC", 8},
    {"CBAT", 18},
    {"CREP", 18},
    {"cSAI", 18},
};

bool get_underlying_asset_decimals(char *compound_ticker, uint8_t *out_decimals) {
    for (size_t i = 0; i < NUM_COMPOUND_BINDINGS; i++) {
        underlying_asset_decimals_t *binding =
            (underlying_asset_decimals_t *) PIC(&UNDERLYING_ASSET_DECIMALS[i]);
        if (strncmp(binding->c_ticker,
                    compound_ticker,
                    strnlen(binding->c_ticker, MAX_TICKER_LEN)) == 0) {
            *out_decimals = binding->decimals;
            return true;
        }
    }
    return false;
}

void
// List of selectors supported by this plugin.
// EDIT THIS: Adapt the variable names and change the `0x` values to match your selectors.

/* From contract: https://etherscan.io/address/0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4#code */
static const uint8_t COMPOUND_APPROVE_SELECTOR[SELECTOR_SIZE] = {0x09, 0x5e, 0xa7, 0xb3};
static const uint8_t COMPOUND_REDEEM_UNDERLYING_SELECTOR[SELECTOR_SIZE] = {0x85, 0x2a, 0x12, 0xe3};
static const uint8_t COMPOUND_REDEEM_SELECTOR[SELECTOR_SIZE] = {0xdb, 0x00, 0x6a, 0x75};
static const uint8_t COMPOUND_MINT_SELECTOR[SELECTOR_SIZE] = {0xa0, 0x71, 0x2d, 0x68};

static const uint8_t COMPOUND_BORROW_SELECTOR[SELECTOR_SIZE] = {0xc5, 0xeb, 0xea, 0xec};
static const uint8_t COMPOUND_REPAY_BORROW_SELECTOR[SELECTOR_SIZE] = {0x0e, 0x75, 0x27, 0x02};
static const uint8_t COMPOUND_TRANSFER_SELECTOR[SELECTOR_SIZE] = {0xa9, 0x05, 0x9c, 0xbb};
static const uint8_t COMPOUND_REPAY_BORROW_ON_BEHALF_SELECTOR[SELECTOR_SIZE] = {0x26,
                                                                                0x08,
                                                                                0xf8,
                                                                                0x18};
static const uint8_t COMPOUND_LIQUIDATE_BORROW_SELECTOR[SELECTOR_SIZE] = {0xf5, 0xe3, 0xc4, 0x62};
static const uint8_t COMPOUND_VOTE_DELEGATE_SELECTOR[SELECTOR_SIZE] = {0x5c, 0x19, 0xa9, 0x5c};
// function `deletegateBySig`
static const uint8_t COMPOUND_MANUAL_VOTE_SELECTOR[SELECTOR_SIZE] = {0x15, 0x37, 0x3e, 0x3d};
static const uint8_t CETH_MINT_SELECTOR[SELECTOR_SIZE] = {0x12, 0x49, 0xc5, 0x8b};

// Array of all the different boilerplate selectors. Make sure this follows the same order as
// the enum defined in `compound_plugin.h` EDIT THIS: Use the names of the array declared above.
const uint8_t *const COMPOUND_SELECTORS[NUM_SELECTORS] = {COMPOUND_MINT_SELECTOR,
                                                          COMPOUND_REDEEM_SELECTOR,
                                                          COMPOUND_REDEEM_UNDERLYING_SELECTOR,
                                                          COMPOUND_BORROW_SELECTOR,
                                                          COMPOUND_REPAY_BORROW_SELECTOR,
                                                          COMPOUND_REPAY_BORROW_ON_BEHALF_SELECTOR,
                                                          COMPOUND_TRANSFER_SELECTOR,
                                                          COMPOUND_LIQUIDATE_BORROW_SELECTOR,
                                                          COMPOUND_MANUAL_VOTE_SELECTOR,
                                                          COMPOUND_VOTE_DELEGATE_SELECTOR,
                                                          CETH_MINT_SELECTOR};

// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters) {
    switch (message) {
     case ETH_PLUGIN_INIT_CONTRACT: {
            ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;
            compound_parameters_t *context = (compound_parameters_t *) msg->pluginContext;
            size_t i;
            for (i = 0; i < NUM_COMPOUND_SELECTORS; i++) {
                if (memcmp((uint8_t *) PIC(COMPOUND_SELECTORS[i]), msg->selector, SELECTOR_SIZE) ==
                    0) {
                    context->selectorIndex = i;
                    break;
                }
            }
            // enforce that ETH amount should be 0, except in ceth.mint case
            if (!allzeroes(msg->pluginSharedRO->txContent->value.value, 32)) {
                if (context->selectorIndex != CETH_MINT) {
                    PRINTF("Eth amount is not zero and token minted is not CETH!\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    break;
                }
            }
            if (i == NUM_COMPOUND_SELECTORS) {
                PRINTF("Unknown selector %.*H\n", SELECTOR_SIZE, msg->selector);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            if (msg->dataSize != COMPOUND_EXPECTED_DATA_SIZE[context->selectorIndex]) {
                PRINTF("Unexpected data size for command %d expected %d got %d\n",
                       context->selectorIndex,
                       COMPOUND_EXPECTED_DATA_SIZE[context->selectorIndex],
                       msg->dataSize);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            if (context->selectorIndex == CETH_MINT) {
                // ETH amount 0x1234 is stored 0x12340000...000 instead of 0x00....001234, so we
                // strip the following zeroes when copying
                memset(context->amount, 0, sizeof(context->amount));
                memmove(context->amount + sizeof(context->amount) -
                            msg->pluginSharedRO->txContent->value.length,
                        msg->pluginSharedRO->txContent->value.value,
                        msg->pluginSharedRO->txContent->value.length);
            }
            PRINTF("compound plugin inititialized\n");
            msg->result = ETH_PLUGIN_RESULT_OK;
        } break;

        case ETH_PLUGIN_PROVIDE_PARAMETER: {
            ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
            compound_parameters_t *context = (compound_parameters_t *) msg->pluginContext;
            PRINTF("compound plugin provide parameter %d %.*H\n",
                   msg->parameterOffset,
                   32,
                   msg->parameter);
            if (context->selectorIndex != CETH_MINT) {
                switch (msg->parameterOffset) {
                    case 4:
                        memmove(context->amount, msg->parameter, 32);
                        msg->result = ETH_PLUGIN_RESULT_OK;
                        break;
                    default:
                        PRINTF("Unhandled parameter offset\n");
                        msg->result = ETH_PLUGIN_RESULT_ERROR;
                        break;
                }
            } else {
                PRINTF("CETH contract expects no parameters\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }
        } break;

        case ETH_PLUGIN_FINALIZE: {
            ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
            PRINTF("compound plugin finalize\n");
            msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
            msg->numScreens = 2;
            msg->uiType = ETH_UI_TYPE_GENERIC;
            msg->result = ETH_PLUGIN_RESULT_OK;
        } break;

        case ETH_PLUGIN_PROVIDE_INFO: {
            ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
            compound_parameters_t *context = (compound_parameters_t *) msg->pluginContext;
            PRINTF("compound plugin provide token: %d\n", (msg->item1 != NULL));
            if (msg->item1 != NULL) {
                strlcpy(context->ticker_1, msg->item1->token.ticker, MAX_TICKER_LEN);
                switch (context->selectorIndex) {
                    case COMPOUND_REDEEM_UNDERLYING:
                    case COMPOUND_MINT:
                    case CETH_MINT:
                        msg->result =
                            get_underlying_asset_decimals(context->ticker_1, &context->decimals)
                                ? ETH_PLUGIN_RESULT_OK
                                : ETH_PLUGIN_RESULT_FALLBACK;
                        break;

                    // Only case where we use the compound contract decimals
                    case COMPOUND_REDEEM:
                        context->decimals = msg->item1->token.decimals;
                        msg->result = ETH_PLUGIN_RESULT_OK;
                        break;

                    default:
                        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
                        break;
                }
            } else {
                msg->result = ETH_PLUGIN_RESULT_FALLBACK;
            }
        } break;

        case ETH_PLUGIN_QUERY_CONTRACT_ID: {
            ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
            compound_parameters_t *context = (compound_parameters_t *) msg->pluginContext;
            strlcpy(msg->name, "Type", msg->nameLength);
            switch (context->selectorIndex) {
                case COMPOUND_REDEEM_UNDERLYING:
                case COMPOUND_REDEEM:
                    strlcpy(msg->version, "Redeem", msg->versionLength);
                    break;

                case COMPOUND_MINT:
                case CETH_MINT:
                    strlcpy(msg->version, "Lend", msg->versionLength);
                    break;

                default:
                    break;
            }
            strlcat(msg->version, " Assets", msg->versionLength);
            msg->result = ETH_PLUGIN_RESULT_OK;
        } break;

        case ETH_PLUGIN_QUERY_CONTRACT_UI: {
            ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
            compound_parameters_t *context = (compound_parameters_t *) msg->pluginContext;
            switch (msg->screenIndex) {
                case 0: {
                    strlcpy(msg->title, "Amount", msg->titleLength);
                    char *ticker_ptr = context->ticker_1;
                    /* skip "c" in front of cToken unless we use "redeem", as
                    redeem is the only operation dealing with a cToken amount */
                    if (context->selectorIndex != COMPOUND_REDEEM) {
                        ticker_ptr++;
                    }
                    amountToString(context->amount,
                                   sizeof(context->amount),
                                   context->decimals,
                                   ticker_ptr,
                                   msg->msg,
                                   100);
                    msg->result = ETH_PLUGIN_RESULT_OK;
                } break;

                case 1:
                    strlcpy(msg->title, "Contract", msg->titleLength);
                    strlcpy(msg->msg, "Compound ", msg->msgLength);
                    strlcat(msg->msg,
                            context->ticker_1 + 1,
                            msg->msgLength);  // remove the 'c' char at beginning of compound ticker
                    msg->result = ETH_PLUGIN_RESULT_OK;
                    break;
                default:
                    break;
            }
        } break;

        default:
            PRINTF("Unhandled message %d\n", message);
    }
}

// Calls the ethereum app.
void call_app_ethereum() {
    unsigned int libcall_params[3];
    libcall_params[0] = (unsigned int) "Ethereum";
    libcall_params[1] = 0x100;
    libcall_params[2] = RUN_APPLICATION;
    os_lib_call((unsigned int *) &libcall_params);
}

// Weird low-level black magic. No need to edit this.
__attribute__((section(".boot"))) int main(int arg0) {
    // Exit critical section
    __asm volatile("cpsie i");

    // Ensure exception will work as planned
    os_boot();

    // Try catch block. Please read the docs for more information on how to use those!
    BEGIN_TRY {
        TRY {
            // Low-level black magic.
            check_api_level(CX_COMPAT_APILEVEL);

            // Check if we are called from the dashboard.
            if (!arg0) {
                // Called from dashboard, launch Ethereum app
                call_app_ethereum();
                return 0;
            } else {
                // Not called from dashboard: called from the ethereum app!
                unsigned int *args = (unsigned int *) arg0;

                // If `ETH_PLUGIN_CHECK_PRESENCE` is set, this means the caller is just trying to
                // know whether this app exists or not. We can skip `dispatch_plugin_calls`.
                if (args[0] != ETH_PLUGIN_CHECK_PRESENCE) {
                    dispatch_plugin_calls(args[0], (void *) args[1]);
                }

                // Call `os_lib_end`, go back to the ethereum app.
                os_lib_end();
            }
        }
        FINALLY {
        }
    }
    END_TRY;

    // Will not get reached.
    return 0;
}
