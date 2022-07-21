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
                           msg->msgLength);
            break;
        case COMPOUND_REDEEM:
            strlcpy(msg->title, "Redeem.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        case COMPOUND_REDEEM_UNDERLYING:
            strlcpy(msg->title, "Redeem underlying.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        case COMPOUND_BORROW:
            strlcpy(msg->title, "Borrow amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        case COMPOUND_REPAY_BORROW:
            strlcpy(msg->title, "Repay borrow.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
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
                           msg->msgLength);
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
                           msg->msgLength);
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
                           msg->msgLength);
            break;
        case COMPOUND_TRANSFER:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        case COMPOUND_MANUAL_VOTE:
            strlcpy(msg->title, "Support.", msg->titleLength);
            amountToString(context->support,
                           sizeof(context->support),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
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

    switch (msg->screenIndex) {
        case 0:
            set_first_param_ui(msg, context);
            break;
        case 1:
            set_second_param_ui(msg, context);
            break;
        case 2:
            set_third_param_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
