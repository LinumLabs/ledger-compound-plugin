import { processTest, populateTransaction } from "./test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x5d3a536e4d6dbd6114cc1ead35777bab948e3643";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f8920182011d844ead9a00850ab5d04c0083086470945d3a536e4d6dbd6114cc1ead35777bab948e364380a4852a12e30000000000000000000000000000000000000000000000002dba2c2dad4b0996c080a00c071301b9e64c3b61d359c1a465c75b2f1df7d805e614325e3a1eb035c4f73fa03fa4b3a4d8a51ea775180ada6f1e0731fa17712dc0f36720d056c871bf2079a9";

// Reference transaction for this test:
// https://etherscan.io/tx/0x4e5e24992a26d3e9f7e0370e6d6d294f7ec5854c60bd9073ad9e9272ed11b494
const testLabel = "redeem_underlying" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "redeem_underlying"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "cUSDT";
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




