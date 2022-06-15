import "core-js/stable";
import "regenerator-runtime/runtime";
import Eth from "@ledgerhq/hw-app-eth";
import Zemu from "@zondax/zemu";
import { expect } from "../jest";

const {NANOS_ETH_ELF_PATH, NANOX_ETH_ELF_PATH, NANOS_COMPOUND_LIB, NANOX_COMPOUND_LIB, sim_options_nanos, sim_options_nanox, TIMEOUT, getTmpPath} = require("generic.js");

const ORIGINAL_SNAPSHOT_PATH_PREFIX = "snapshots/repay/";

const ORIGINAL_SNAPSHOT_PATH_NANOS = ORIGINAL_SNAPSHOT_PATH_PREFIX + "nanos/";
const ORIGINAL_SNAPSHOT_PATH_NANOX = ORIGINAL_SNAPSHOT_PATH_PREFIX + "nanox/";

test("repay nanos", async () => {
  jest.setTimeout(TIMEOUT);

  const sim = new Zemu(NANOS_ETH_ELF_PATH, NANOS_COMPOUND_LIB);

  let tmpPath = getTmpPath(expect.getState().currentTestName);

  try {
    await sim.start(sim_options_nanos);

    let transport = await sim.getTransport();
    const eth = new Eth(transport);

    // Send transaction
    let tx = eth.signTransaction("44'/60'/0'/1/0", "F8494A8502540BE400830222E0947F39C581F595B53C5CB19BD0B3F8DA6C935E2CA080A4EA598CB0000000000000000000000000000000000000000000000000016345785D89FFFF018080");

    let filename;

    await sim.waitUntilScreenIsNot(sim.getMainMenuSnapshot());
    // review tx
    filename = "review.png";
    await sim.snapshot(tmpPath + filename);
    const review = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_review = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(review).toMatchSnapshot(expected_review);

    // Compound message
    filename = "compound.png";
    await sim.clickRight(tmpPath + filename);
    const compound = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_compound = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(compound).toMatchSnapshot(expected_compound);

    // repay
    filename = "repay.png";
    await sim.clickRight(tmpPath + filename);
    const repay = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_repay = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(repay).toMatchSnapshot(expected_repay);

    // Max Fees
    filename = "fees.png";
    await sim.clickRight(tmpPath + filename);
    const fees = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_fees = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(fees).toMatchSnapshot(expected_fees);

    // Accept
    filename = "accept.png";
    await sim.clickRight(tmpPath + filename);
    const accept = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_accept = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(accept).toMatchSnapshot(expected_accept);

    await sim.clickBoth();

    await expect(tx).resolves.toEqual(
      {
        'r': 'fc7934a8c6bef19d4469594c16c4fe9bf67f2ef39436aac9223629e25e43da95',
        's': '10efe59dd11ef682acdeb74ad0f9139c10aaa77646f78b2e76ddf77534433180',
        'v': '25'
      }
    );
  } finally {
    await sim.close();
  }
});

test("repay nanox", async () => {
  jest.setTimeout(TIMEOUT);

  const sim = new Zemu(NANOX_ETH_ELF_PATH, NANOX_COMPOUND_LIB);

    let tmpPath = getTmpPath(expect.getState().currentTestName);

  try {
    await sim.start(sim_options_nanox);

    let transport = await sim.getTransport();
    const eth = new Eth(transport);

    // Send transaction
    let tx = eth.signTransaction("44'/60'/0'/1/0", "F8494A8502540BE400830222E0947F39C581F595B53C5CB19BD0B3F8DA6C935E2CA080A4EA598CB0000000000000000000000000000000000000000000000000016345785D89FFFF018080");

    let filename;

    await sim.waitUntilScreenIsNot(sim.getMainMenuSnapshot());
    // Review tx
    filename = "review.png";
    await sim.snapshot(tmpPath + filename);
    const review = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_review = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOX + filename);
    expect(review).toMatchSnapshot(expected_review);

    // Compound message
    filename = "compound.png";
    await sim.clickRight(tmpPath + filename);
    const lido = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_lido = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOX + filename);
    expect(lido).toMatchSnapshot(expected_lido);

    // repay
    filename = "repay.png";
    await sim.clickRight(tmpPath + filename);
    const repay = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_repay = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOX + filename);
    expect(repay).toMatchSnapshot(expected_repay);

    // Max Fees
    filename = "fees.png";
    await sim.clickRight(tmpPath + filename);
    const fees = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_fees = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOX + filename);
    expect(fees).toMatchSnapshot(expected_fees);

    // Accept
    filename = "accept.png";
    await sim.clickRight(tmpPath + filename);
    const accept = Zemu.LoadPng2RGB(tmpPath + filename);
    const expected_accept = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOX + filename);
    expect(accept).toMatchSnapshot(expected_accept);

    await sim.clickBoth();

    await expect(tx).resolves.toEqual(
      {
        'r': 'fc7934a8c6bef19d4469594c16c4fe9bf67f2ef39436aac9223629e25e43da95',
        's': '10efe59dd11ef682acdeb74ad0f9139c10aaa77646f78b2e76ddf77534433180',
        'v': '25'
      }
    );
  } finally {
    await sim.close();
  }
});
