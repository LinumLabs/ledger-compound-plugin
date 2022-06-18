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

#define NUM_SELECTORS 11
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

// Array of all the different boilerplate selectors. Make sure this follows the same order as
// the enum defined in `compound_plugin.h` EDIT THIS: Use the names of the array declared above.
const uint8_t *const COMPOUND_SELECTORS[NUM_SELECTORS] = {
    COMPOUND_REDEEM_UNDERLYING_SELECTOR,
    COMPOUND_REDEEM_SELECTOR,
    COMPOUND_MINT_SELECTOR,
    COMPOUND_BORROW_SELECTOR,
    COMPOUND_REPAY_BORROW_SELECTOR,
    COMPOUND_REPAY_BORROW_ON_BEHALF_SELECTOR,
    COMPOUND_TRANSFER_SELECTOR,
    COMPOUND_LIQUIDATE_BORROW_SELECTOR,
    COMPOUND_MANUAL_VOTE_SELECTOR,
    COMPOUND_VOTE_DELEGATE_SELECTOR,
};

// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters) {
    switch (message) {
        case ETH_PLUGIN_INIT_CONTRACT:
            handle_init_contract(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_PARAMETER:
            handle_provide_parameter(parameters);
            break;
        case ETH_PLUGIN_FINALIZE:
            handle_finalize(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_INFO:
            handle_provide_token(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_ID:
            handle_query_contract_id(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            handle_query_contract_ui(parameters);
            break;
        default:
            PRINTF("Unhandled message %d\n", message);
            break;
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
