import { processTest, populateTransaction } from "./test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f892018201b784773594008507a56103b88304e9e49470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4852a12e30000000000000000000000000000000000000000000000056bc75e2d63100000c001a08800fa6c988479a08b91b4466aab86b5384e9d4e83ece9de05e4826a0e89f0bea07c0488a3da0c981942bdf1637a9b95f9d6acf39e84195413e5093e6b22c70b26";

// Reference transaction for this test:
// https://etherscan.io/tx/0x4e5e24992a26d3e9f7e0370e6d6d294f7ec5854c60bd9073ad9e9272ed11b494
const testLabel = "redeem underlying" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "redeem_underlying"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "cCOMP";
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




