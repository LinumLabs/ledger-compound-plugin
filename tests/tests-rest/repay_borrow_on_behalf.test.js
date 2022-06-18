import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0x0160b3aec12fd08e6be0040616c7c38248efb4413168a3372fc4d2db0e5961bb

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Repay borrow on behalf', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xb27a69cd3190ad0712da39f6b809ecc019ecbc319d3c17169853270226d18a8a
  //  const tx = eth.signTransaction(
  //    "44'/60'/0'/0",
  //    "02f892018202f1843b9aca00850a5dce1bba830426949470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4a0712d68000000000000000000000000000000000000000000000002718291b0154533abc080a06ea774b9c0ba48ec92107955d01487e65cae5ce74505fafe17d38fcb12c4de52a0784495c38a539c9c584dee59ad2a012186c9e8977c85f9cc78ef7f6ca5c54c24",
  //  );
 
  //  const right_clicks = model.letter === 'S' ? 12 : 6;
 
  //  // Wait for the application to actually load and parse the transaction
  //  await waitForAppScreen(sim);
  //  // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
  //  await sim.navigateAndCompareSnapshots('.', model.name + 'repay_borrow_on_behalf', [right_clicks, 0]);
 
  //  await tx;
   }));
});