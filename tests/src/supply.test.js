import "core-js/stable";
import "regenerator-runtime/runtime";
import {
  waitForAppScreen,
  zemu,
  genericTx,
  nano_models,
  txFromEtherscan,
} from "./test.fixture";
import { ethers } from "ethers";
import { parseUnits } from "ethers/lib/utils";

// EDIT THIS: Replace with your contract address
const contractAddr = "0xF25212E676D1F7F89Cd72fFEe66158f541246445";
// EDIT THIS: Replace `boilerplate` with your plugin name
const pluginName = "compound";
const testNetwork = "ethereum";
const abi_path =
  `../networks/${testNetwork}/${pluginName}/abis/` + contractAddr + ".json";
const abi = require(abi_path);

// Test from replayed transaction: https://polygonscan.com/tx/0xa1cf1dccd358b8a5b3effdee819f365a2597930e06ae2224926a714b1787e1e6
// EDIT THIS: build your own test
nano_models.forEach(function (model) {
  test(
    "[Nano " + model.letter + "] Supply USDC For cUSDC ",
    zemu(model, async (sim, eth) => {
      // The rawTx of the tx up above is accessible through, Feature not possible: https://polygonscan.io/getRawTx?tx=0xa1cf1dccd358b8a5b3effdee819f365a2597930e06ae2224926a714b1787e1e6
      const serializedTx = txFromEtherscan(
        "0x02f9015a0181d38459682f0085215d7c1e598302a4e9947a250d5630b4cf539739df2c5dacb4c659f2488d88016345785d8a0000b8e47ff36ab50000000000000000000000000000000000000000000000018b1dd9dc51b5a9f7000000000000000000000000000000000000000000000000000000000000008000000000000000000000000015557c8b7246c38ee71ea6dc69e4347f5dac210400000000000000000000000000000000000000000000000000000000615336100000000000000000000000000000000000000000000000000000000000000002000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc20000000000000000000000006b3595068778dd592e39a122f4f5a5cf09c90fe2c001a089c5ce4ce199f7d93ea1d54c08133fab9407d8de63a9885d59d8ce69a59573dda045f6a0e0d3288dfdfddc23ad0afb9577c4011801f598d581a46cd0b0e2bd0571"
      );

      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      const right_clicks = model.letter === "S" ? 12 : 6;

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);
      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_supply_usdc_cusdc",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});

// Test from constructed transaction
// EDIT THIS: build your own test
nano_models.forEach(function (model) {
  test(
    "[Nano " + model.letter + "] Supply USDC For cUSDC",
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      // Constants used to create the transaction
      // EDIT THIS: Remove what you don't need
      const amount = parseUnits("1000000", "wei");
      const asset = "0x2791bca1f2de4661ed88a30c99a7a9449aa84174";

      const { data } = await contract.populateTransaction.supply(asset, amount);

      // Get the generic transaction template
      let unsignedTx = genericTx;
      // Modify `to` to make it interact with the contract
      unsignedTx.to = contractAddr;
      // Modify the attached data
      unsignedTx.data = data;

      // Create serializedTx and remove the "0x" prefix
      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);

      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      const right_clicks = model.letter === "S" ? 7 : 5;

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);
      // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
      // EDIT THIS: modify `10` to fix the number of screens you are expecting to navigate through.
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_supply_usdc_cusdc",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
