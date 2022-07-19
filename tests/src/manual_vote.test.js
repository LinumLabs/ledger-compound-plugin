import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0xc0da01a04c3f3e0be433606045bb7017a7323e38";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json'
;// Reference transaction for this test:
// https://etherscan.io/tx/0xa9680976fbebfb6b5777d6b5545769eb3f112da75b8fc9c3c7546517cbffb755
const rawTxHex = "0xf8ab81a08513ca6512008301195494c0da01a04c3f3e0be433606045bb7017a7323e3880b84415373e3d000000000000000000000000000000000000000000000000000000000000002a000000000000000000000000000000000000000000000000000000000000000126a04aec2c90c0648c84bd966601df71b94d30c2a1d33591e622b8c89c2b6560eecba073e13e16b8879ed8fc5fc979c5d4b908b31aba1d25c4bedac04d6fd859688806";
const testLabel = "manual vote" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "manual_vote"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "Compound";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 7
  }
];

devices.forEach((device) => processTest(device, contractName, testLabel, testLabel, rawTxHex, signedPlugin, "", testNetwork));




