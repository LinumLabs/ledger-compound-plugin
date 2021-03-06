import "core-js/stable";
import "regenerator-runtime/runtime";
import { txFromEtherscan, zemu, transactionUploadDelay} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0xc00e94cb662c3520282e6f5717214004a7f26888";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f893018201378503f5476a008503f5476a008301b59494c00e94cb662c3520282e6f5717214004a7f2688880a45c19a95c000000000000000000000000501783e585936116220b5028c4c22dc9fdb991bcc001a0b7550056af6f8bdd1ddb570b2eded0cb61ee7067edfcaf3111cf90c5cf5e1009a007427fb21c7bacb3f2edeed79827add2054441e6687d33bc38c59ef7633c409a";
const testLabel = "delegate";
const testNetwork = "ethereum";
const signed = false;
const contractName = "Compound"
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 8
  }
];
// Reference transaction for this test:
// https://etherscan.io/tx/0xc00e94cb662c3520282e6f5717214004a7f26888
// The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xc00e94cb662c3520282e6f5717214004a7f26888

const processTransaction = async(eth, sim, steps, label, rawTxHex) => {

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
