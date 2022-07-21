import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f890012a843b9aca00850501664ca28302ee149470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a40e75270200000000000000000000000000000000000000000000000000f6a2f4cebcbceec001a09de57b53d91317ca60feaa9dc3ab43e4986c6bbc306639bc4d4ba98230ceb8d3a00c490facdc298b31cf6d96636be77d1c1e6d80c1f7a7262ef9ce0acf95a1dc23";
// Reference transaction for this test:
// https://etherscan.io/tx/0x209213e8266e50774e2b70170c4427684628e3bbbc5fe0b5b2bab41888b8fc6c
const testLabel = "repay borrow" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "repay_borrow"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "cCOMP";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 9
  }
];

devices.forEach((device) => processTest(device, contractName, testLabel, testDirSuffix, rawTxHex, signedPlugin, "", testNetwork));




