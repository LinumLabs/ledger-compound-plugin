import "core-js/stable";
import "regenerator-runtime/runtime";
import { txFromEtherscan, zemu, transactionUploadDelay} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f892018202f1843b9aca00850a5dce1bba830426949470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4a0712d68000000000000000000000000000000000000000000000002718291b0154533abc080a06ea774b9c0ba48ec92107955d01487e65cae5ce74505fafe17d38fcb12c4de52a0784495c38a539c9c584dee59ad2a012186c9e8977c85f9cc78ef7f6ca5c54c24";
const testLabel = "liquidate_borrow";
const testNetwork = "ethereum";
const signed = false;
const contractName = "Compound"
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 6
  }
];
// Reference transaction for this test:
// https://etherscan.io/tx/0x0160b3aec12fd08e6be0040616c7c38248efb4413168a3372fc4d2db0e5961bb

const processTransaction = async (eth, sim, steps, label, rawTxHex) => {
  const serializedTx = txFromEtherscan(rawTxHex);
  let tx = eth.signTransaction("44'/60'/0'/0/0", serializedTx);

  await sim.waitUntilScreenIsNot(
    sim.getMainMenuSnapshot(),
    transactionUploadDelay
  );
  await sim.navigateAndCompareSnapshots(".", label, [steps, 0]);

  await tx;
}

devices.forEach(async (device) =>    
  test(
    "[" + contractName + "] - " + device.label + " - " + testLabel,
    zemu(device.name, async (sim, eth) => {
      await processTransaction(
        eth,
        sim,
        device.steps,
        testLabel,
        rawTxHex
      );
    },signed, testNetwork)
  )
);
