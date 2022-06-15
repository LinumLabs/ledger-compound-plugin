import { processTest, populateTransaction } from "./test.fixture";

// Reference transaction for this test:
// https://etherscan.io/tx/0xa26b900bd6de31f61e673c4f424f952bf9b0e94ece49b09dd5e8dccb198478af

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";

const rawTxHex = "0x02f8b1013e84773594008504a817c8008305152e9470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480b844a9059cbb000000000000000000000000fc91dc54f06a25f16d83fb07c7d2ea78d57d345a000000000000000000000000000000000000000000000000000000044d7fae3cc080a0b07f49bdaab4de25c03c93e0068679dc1af77120c78d179bb801574f9d178001a07f02a89716210e7ceaba9db71d0c01b99b686649088107d979a0e7577c342031";

const testLabel = "transferTokens" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "transfer"; // <= directory to compare device snapshots to
const signedPlugin = false;
const contractName = "Compound";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 6
  }
];

devices.forEach((device) =>{
  processTest(device, contractName, testLabel, testDirSuffix, "", signedPlugin, serializedTx, testNetwork);
}
);



