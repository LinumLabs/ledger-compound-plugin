import "core-js/stable";
import "regenerator-runtime/runtime";
import { txFromEtherscan, zemu, transactionUploadDelay} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0xc0da01a04c3f3e0be433606045bb7017a7323e38";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0xf8ab81a08513ca6512008301195494c0da01a04c3f3e0be433606045bb7017a7323e3880b84415373e3d000000000000000000000000000000000000000000000000000000000000002a000000000000000000000000000000000000000000000000000000000000000126a04aec2c90c0648c84bd966601df71b94d30c2a1d33591e622b8c89c2b6560eecba073e13e16b8879ed8fc5fc979c5d4b908b31aba1d25c4bedac04d6fd859688806";
const testLabel = "manual_vote";
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
// https://etherscan.io/tx/0xa9680976fbebfb6b5777d6b5545769eb3f112da75b8fc9c3c7546517cbffb755

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