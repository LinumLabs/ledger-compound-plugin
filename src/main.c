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
// List of selectors supported by this plugin.
// EDIT THIS: Adapt the variable names and change the `0x` values to match your selectors.

/* From contract: https://etherscan.io/address/0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4#code */
static const uint32_t COMPOUND_APPROVE_SELECTOR = 0x095ea7b3;
static const uint32_t COMPOUND_REDEEM_UNDERLYING_SELECTOR = 0x852a12e3;
static const uint32_t COMPOUND_REDEEM_SELECTOR = 0xdb006a75;
static const uint32_t COMPOUND_MINT_SELECTOR = 0xa0712d68;

static const uint32_t COMPOUND_BORROW_SELECTOR = 0xc5ebeaec;
static const uint32_t COMPOUND_REPAY_BORROW_SELECTOR = 0x0e752702;
static const uint32_t COMPOUND_TRANSFER_SELECTOR = 0xa9059cbb;
static const uint32_t COMPOUND_REPAY_BORROW_ON_BEHALF_SELECTOR = 0x2608f818;
static const uint32_t COMPOUND_LIQUIDATE_BORROW_SELECTOR = 0xf5e3c462;
static const uint32_t COMPOUND_VOTE_DELEGATE_SELECTOR = 0x5c19a95c;
// function `deletegateBySig`
static const uint32_t COMPOUND_MANUAL_VOTE_SELECTOR = 0x15373e3d;
static const uint32_t CETH_MINT_SELECTOR = 0x1249c58b;

// Array of all the different boilerplate selectors. Make sure this follows the same order as
// the enum defined in `compound_plugin.h` EDIT THIS: Use the names of the array declared above.
const uint32_t *const COMPOUND_SELECTORS[NUM_SELECTORS] = {COMPOUND_MINT_SELECTOR,
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
    PRINTF("Message");
    PRINTF(message);
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
