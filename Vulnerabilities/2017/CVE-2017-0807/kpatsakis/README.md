# Proof of concept of CVE-2017-0807
This is a demo application with deliberately sloppy interface for the CVE-2017-0807 reported by Efthimios Alepis and Constantinos Patsakis.
The vulnerability illustrates that due to security issues in every Android version up to Nougat, an unprivileged user can overlay almost every Android interface and trick the user into getting his input. In the demo we overlay a screen which makes our app administrator of the device, however, there are numerous other possibilities to exploit this vulnerability. Contrary to other attacks, e.g. cloak and dagger our attack does not request any dangerous or system permission like SYSTEM ALERT WINDOW.
A video which showcases the issue can be found [here](https://www.youtube.com/watch?v=zX4KckkNGdQ).
For more details the interested reader may refer to:
*Alepis, Efthimios, and Constantinos Patsakis. "Trapped by the UI: The Android case." International Symposium on Research in Attacks, Intrusions, and Defenses. Springer, Cham, 2017.* [Link](https://link.springer.com/chapter/10.1007/978-3-319-66332-6_15)
# External links
[NIST](https://nvd.nist.gov/vuln/detail/CVE-2017-0807)
[Pixel / Nexus Security Bulletin—October 2017](https://source.android.com/security/bulletin/pixel/2017-10-01)

This work was supported by the European Commission under the Horizon 2020 Programme (H2020), as part of the [OPERANDO](https://www.operando.eu) project (Grant Agreement no. 653704) and is based upon work from COST Action [CRYPTACUS](https://www.cryptacus.eu), supported by COST (European Cooperation in Science and Technology).
