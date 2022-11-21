# Proof of Concept for CVE-2022-36532

A vulnerability in Bolt CMS version 5.1.12 and below allows an authenticated user with the `EDITOR_ROLE` to achieve remote code execution. This vulnerability can be detected using this script, for details on the vulnerability see <https://lutrasecurity.com/en/articles/cve-2022-36532/>.

## Usage

Three parameters are needed to run the script: The username, the corresponding password and the Bolt CMS instance URL.
To test an instance at `http://127.0.0.1:8000/` with the credentials `jsmith:password` use the following command:

```bash
./CVE-2022-36532.py jsmith password "http://127.0.0.1:8000/"
```

For example:

![CVE-2022-36532 py](https://user-images.githubusercontent.com/29411434/188486959-d385d80e-4db7-4072-b804-643a893a6d1a.png)
