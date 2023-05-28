import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x5d3a536e4d6dbd6114cc1ead35777bab948e3643";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f890011a843b9aca00850fe8f7a25a8302ee14945d3a536e4d6dbd6114cc1ead35777bab948e364380a40e752702000000000000000000000000000000000000000000000057c423722b9d6c0000c080a02981227a28550cec31fd4bc4db2cdb899c172ac42b6f07e255675ea5c0121e3da0057dd79902a4b7ceca2d678306f0cc43f575e8e182b6f2bbe51a8c8502df3392";
// Reference transaction for this test:
// https://etherscan.io/tx/0x209213e8266e50774e2b70170c4427684628e3bbbc5fe0b5b2bab41888b8fc6c
const testLabel = "repay borrow" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "repay_borrow"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "cDAI";
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




