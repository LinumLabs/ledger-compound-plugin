#include "compound_plugin.h"

// One param functions handler
void handle_one_param_function(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case MINT_AMOUNT:  // mintAmount
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case REDEEM_TOKENS:
            copy_parameter(context->amount, msg->parameter, 32);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case REDEEM_AMOUNT:
            copy_parameter(context->amount, msg->parameter, 32);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case BORROW_AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case REPAY_AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case DELEGATEE:
            copy_parameter(context->dest,
                   &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
                   sizeof(context->dest));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case CETH_AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Repay borrow on behalf handler
void repay_borrow_on_behalf(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case BORROWER:  // mintAmount
            memcpy(context->dest,
                   &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
                   sizeof(context->dest));
            context->next_param = REPAY_AMOUNT;
            break;
        case REPAY_AMOUNT:
            memcpy(context->amount, msg->parameter, INT256_LENGTH);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Vote cast or Manual vote
void manual_vote(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case PROPOSAL_ID:  // PROPOSAl_ID
            memcpy(context->proposal_id, msg->parameter, INT256_LENGTH);
            context->next_param = SUPPORT;
            break;
        case SUPPORT:
            memcpy(context->support, msg->parameter, INT256_LENGTH);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Transfer function handler
void transfer_tokens(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case RECIPIENT:  // mintAmount
            memcpy(context->dest,
                   &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
                   sizeof(context->dest));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            memcpy(context->amount, msg->parameter, INT256_LENGTH);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Liquidate borrow handler
void liquidate_borrow(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case BORROWER:  // borrower
            memcpy(context->dest,
                   &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
                   sizeof(context->dest));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            memcpy(context->amount, msg->parameter, INT256_LENGTH);
            context->next_param = COLLATERAL;
            break;
        case COLLATERAL:
            memcpy(context->collateral,
                   &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
                   sizeof(context->dest));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           32,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;
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
    PRINTF(context->selectorIndex);

    switch (context->selectorIndex) {
        case COMPOUND_MINT:
            memmove(context->amount, msg->parameter, 32);
            break;
        case COMPOUND_REDEEM:
            memmove(context->amount, msg->parameter, 32);
            break;
        case COMPOUND_REDEEM_UNDERLYING:
             memmove(context->amount, msg->parameter, 32);
            break;
        case COMPOUND_BORROW:
            handle_one_param_function(msg, context);
            break;
        case COMPOUND_REPAY_BORROW:
            handle_one_param_function(msg, context);
            break;
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            repay_borrow_on_behalf(msg, context);
            break;
        case COMPOUND_TRANSFER:
            transfer_tokens(msg, context);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            liquidate_borrow(msg, context);
            break;
        case COMPOUND_MANUAL_VOTE:
            manual_vote(msg, context);
            break;
        case COMPOUND_VOTE_DELEGATE:
            handle_one_param_function(msg, context);
            break;
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
