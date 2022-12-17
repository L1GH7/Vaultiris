| Date                  | Description                                                  |
| --------------------- | ------------------------------------------------------------ |
| **October 01, 2020**  | DEVCORE started reviewing the security on Microsoft Exchange Server |
| **December 10, 2020** | DEVCORE discovered the first pre-auth proxy bug ([CVE-2021-26855](https://msrc.microsoft.com/update-guide/vulnerability/CVE-2021-26855)) |
| **December 27, 2020** | DEVCORE escalated the first bug to an authentication bypass to become admin |
| **December 30, 2020** | DEVCORE discovered the second post-auth arbitrary-file-write bug ([CVE-2021-27065](https://msrc.microsoft.com/update-guide/vulnerability/CVE-2021-27065)) |
| **December 31, 2020** | DEVCORE chained all bugs together to a workable pre-auth RCE exploit |
| **January 05, 2021**  | DEVCORE sent (18:41 GMT+8) the advisory and exploit to Microsoft through the MSRC portal directly |
| **January 06, 2021**  | MSRC acknowledged the pre-auth proxy bug (MSRC case 62899)   |
| **January 06, 2021**  | MSRC acknowledged the post-auth arbitrary-file-write bug (MSRC case 63835) |
| **January 08, 2021**  | MSRC confirmed the reported behavior                         |
| **January 11, 2021**  | DEVCORE attached a 120-days public disclosure deadline to MSRC and checked for bug collision |
| **January 12, 2021**  | MSRC flagged the intended deadline and confirmed no collision at that time |
| **February 02, 2021** | DEVCORE checked for the update                               |
| **February 02, 2021** | MSRC replied "they are splitting up different aspects for review individually and got at least one fix which should meet our deadline" |
| **February 12, 2021** | MSRC asked the title for acknowledgements and whether we will publish a blog |
| **February 13, 2021** | DEVCORE confirmed to publish a blog and said will postpone the technique details for two weeks, and will publish an easy-to-understand advisory (without technique details) instead |
| **February 18, 2021** | DEVCORE provided the advisory draft to MSRC and asked for the patch date |
| **February 18, 2021** | MSRC pointed out a minor typo in our draft and confirmed the patch date is 3/9 |
| **February 27, 2021** | MSRC said they are almost set for release and wanted to ask if we're fine with being mentioned in their advisory |
| **February 28, 2021** | DEVCORE agreed to be mentioned in their advisory             |
| **March 03, 2021**    | MSRC said they are likely going to be pushing out their blog earlier than expected and won’t have time to do an overview of the blog |
| **March 03, 2021**    | MSRC published the patch and advisory and acknowledged DEVCORE officially |
| **March 03, 2021**    | DEVCORE has launched an initial investigation after informed of [active exploitation advisory from Volexity](https://www.volexity.com/blog/2021/03/02/active-exploitation-of-microsoft-exchange-zero-day-vulnerabilities/) |
| **March 04, 2021**    | DEVCORE has confirmed the in-the-wild exploit was the same one reported to MSRC |
| **March 05, 2021**    | DEVCORE hasn't found concern in the investigation so far     |
| **March 06, 2021**    | [CISA says](https://twitter.com/USCERT_gov/status/1368216461571919877) it is aware of “widespread domestic and international exploitation of Microsoft Exchange Server flaws.” |
| **March 7, 2021**     | Security experts continue effort to notify victims, coordinate remediation, and remain vigilant for “Stage 2” of this attack (further exploitation of already-compromised servers). |
| **March 10, 2021**    | As many as 60,000 Exchange Servers in Germany were initially exposed to the vulnerabilities. Roughly 25,000 of those systems still need to be fixed. |
| **March 10, 2021**    | ESET Research has discovered that more than 10 different advanced persistent threat (APT) groups are exploiting the recent Microsoft Exchange vulnerabilities to compromise email servers. Moreover, ESET has identified more than 5,000 email servers that have been affected by malicious activity related to the incident |

