import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0x02d32caf20582bf1c84c941ef6c80266036034929b6947ef7f781053b411d6da

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Mint cToken', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0x02d32caf20582bf1c84c941ef6c80266036034929b6947ef7f781053b411d6da
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "0x02f890011c843b9aca00850a34c5492683033c349470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4a0712d68000000000000000000000000000000000000000000000005b34717b0eb495ec8c001a0b4eaad3f6f094ee88a7413fd08164416c50e92cf52c7072301f35985df1ebf80a02d59d6d4b3e335433f1bd21b5de29a21de5904e83dc293db34863f19fd7325e2",
   );
 
   const right_clicks = model.letter === 'S' ? 12 : 6;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'mint', [right_clicks, 0]);
 
   await tx;
   }));
});