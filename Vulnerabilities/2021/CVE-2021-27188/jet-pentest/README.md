# CVE-2021-27188

## [Suggested description]
The FX Aggregator terminal client by "Sovremennye Delovye Tekhnologii" allows attackers to cause a denial of service (access suspended for five hours) by making five invalid login attempts to a victim's account.

## [VulnerabilityType Other]
DoS

## [Vendor of Product]
OOO Sovremennye Delovye Tekhnologii

## [Affected Product Code Base]
Fx-agreggator terminal client - 1

## [Affected Component]
affected executable

## [Attack Type]
Remote

## [Impact Denial of Service]
true

## [Attack Vectors]
To exploit vulnerability someone should try to login as other user with invalid credentials for 5 times. Valid user will be blocked for 5 hours.

## [Has vendor confirmed or acknowledged the vulnerability?] 
true

## [Discoverer]
Maria Kononova, Dmitry Kuramin (Jet Infosystems, jet.su)

## [Reference]
https://sdt-fx.ru/
