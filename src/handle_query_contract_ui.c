#include "compound_plugin.h"

// Set UI for "Address" screen.
void set_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    // Prefix the address with `0x`.

    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    switch (context->selectorIndex) {
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            getEthAddressStringFromBinary(context->dest,
                                          (uint8_t *) msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
        case COMPOUND_TRANSFER:
            getEthAddressStringFromBinary(context->dest,
                                          (uint8_t *) msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            getEthAddressStringFromBinary(context->dest,
                                          (uint8_t *) msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
        case COMPOUND_VOTE_DELEGATE:
            getEthAddressStringFromBinary(context->dest,
                                          (uint8_t *) msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
    }
}

// Set UI for First param screen
void set_first_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case COMPOUND_MINT:
            strlcpy(msg->title, "Mint.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_REDEEM:
            strlcpy(msg->title, "Redeem.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_REDEEM_UNDERLYING:
            strlcpy(msg->title, "Redeem underlying.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_BORROW:
            strlcpy(msg->title, "Borrow amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_REPAY_BORROW:
            strlcpy(msg->title, "Repay borrow.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            strlcpy(msg->title, "Borrower.", msg->titleLength);
            set_address_ui(msg, context);
            break;
        case COMPOUND_TRANSFER:
            strlcpy(msg->title, "Recipient.", msg->titleLength);
            set_address_ui(msg, context);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Liquidate borrower.", msg->titleLength);
            set_address_ui(msg, context);
            break;
        case COMPOUND_MANUAL_VOTE:
            strlcpy(msg->title, "Proposal id.", msg->titleLength);
            amountToString(context->proposal_id,
                           sizeof(context->proposal_id),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_VOTE_DELEGATE:
            strlcpy(msg->title, "Delegatee.", msg->titleLength);
            set_address_ui(msg, context);
            break;
        case CETH_MINT:
            strlcpy(msg->title, "Mint cETH", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
    }
}

void set_second_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            strlcpy(msg->title, "Repaying amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_TRANSFER:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
        case COMPOUND_MANUAL_VOTE:
            strlcpy(msg->title, "Support.", msg->titleLength);
            amountToString(context->support,
                           sizeof(context->support),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           100);
            break;
    }
}

void set_third_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case COMPOUND_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Collateral.", msg->titleLength);
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
            // Setting it to `0` will make it work with every chainID :)
            uint64_t chainid = 0;
            getEthAddressStringFromBinary(context->collateral,
                                          (uint8_t *) msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;
    PRINTF(msg->screenIndex);

    switch (msg->screenIndex) {
        case 0:{
            strlcpy(msg->title, "Amountsss", msg->titleLength);
            char *ticker_ptr = context->ticker;
            /* skip "c" in front of cToken unless we use "redeem", as
            redeem is the only operation dealing with a cToken amount */

            const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
            uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;
            uint8_t decimals = context->decimals;
            const char *ticker = context->ticker;
            // Converts the uint256 number located in `eth_amount` to its string representation and
            // copies this to `msg->msg`.
            amountToString(eth_amount, eth_amount_size, decimals, ticker, msg->msg, msg->msgLength);
            msg->result = ETH_PLUGIN_RESULT_OK;
        } break;
        case 1:
            strlcpy(msg->title, "Contract", msg->titleLength);
            strlcpy(msg->msg, "Compound ", msg->msgLength);
            strlcat(msg->msg,
                    context->ticker + 1,
                    msg->msgLength);  // remove the 'c' char at beginning of compound ticker
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
