import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0xc00e94cb662c3520282e6f5717214004a7f26888

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Swap Exact Eth For Tokens with beneficiary', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xc00e94cb662c3520282e6f5717214004a7f26888
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "02f893018201378503f5476a008503f5476a008301b59494c00e94cb662c3520282e6f5717214004a7f2688880a45c19a95c000000000000000000000000501783e585936116220b5028c4c22dc9fdb991bcc001a0b7550056af6f8bdd1ddb570b2eded0cb61ee7067edfcaf3111cf90c5cf5e1009a007427fb21c7bacb3f2edeed79827add2054441e6687d33bc38c59ef7633c409a",
   );
 
   const right_clicks = model.letter === 'S' ? 12 : 6;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'delegate', [right_clicks, 0]);
 
   await tx;
   }));
});