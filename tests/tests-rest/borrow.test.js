import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from '../src/test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0x05a71f11675faa1e43aaeb228d8068d7c851f940f5726c7958dea57a07e1b2bc

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Borrow cCOMP', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/tx/0x05a71f11675faa1e43aaeb228d8068d7c851f940f5726c7958dea57a07e1b2bc
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "02f89001248440207e0785072d303d41830586249470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4c5ebeaec00000000000000000000000000000000000000000000000161e232e52c760000c080a05caafea5cd1bde52232fb2703111af375943420bc2471feeb3c7c773427577d6a028dc272346cdde78da0916fb92464cfe65ddf0910c409c067fb7fd0a5983c6b3",
   );
 
   const right_clicks = model.letter === 'S' ? 12 : 6;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'mint', [right_clicks, 0]);
 
   await tx;
   }));
});