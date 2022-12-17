## Introduction
The application is used for tracking people according to German infection laws to be able to trace Covid-19 infection chains.
It was created by a student project called [iCampus](https://icampus.th-wildau.de) and is used not only at the [university in Wildau](https://www.th-wildau.de/) but also at [Cottbus-Senftenberg](https://www.b-tu.de/). Around 10.000 people are affected. Guests of the universities are required to use this application.

The application consists of a Vue frontend and a Laravel backend.
The backend exposes the guest user registration endpoint `/corona-app-backend/api/account/register`

which allows the registration of guests of the university. Members of the university can login using their single sign on username/password combination.

## Problem description
This registration endpoint is not protected against the automated creation of users (e.g. via a captcha). Once a program obtains a csrf cookie via `/corona-app-backend/sanctum/csrf-cookie` and a session token via `/corona-app-backend/account` it can create users. As many as it wants. As long as it wants. Too many fake users make it practically impossible for a health organisation to trace infection chains making this is a denial of service attack type.

The registration of a user is a post to the registration endpoint containing the csrf cookie and session token with the following json payload:

`{"first_name":"John","last_name":"Doe","telephone":"099182","email":null,"accept":true}`

Either the fields email or phone need to be set. Both are not verified (apart from syntax checks).

With a valid user session visits (checkins into rooms) can be created via the endpoint `/corona-app-backend/api/visit`
A visit is a post to the visit endpoint containing the csrf cookie and session token with the following json payload:

`{"room":"15-K01","visited_on":"2021-05-09T22:01:00.000Z","exited_on":"2021-05-10T21:59:00.000Z"}')`

The room field value can be obtained from a list which can be retrieved from the room endpoint `/corona-app-backend/api/rooms` and delivers a json with all the rooms of the university. One only needs the csrf cookie and the session token for the call to succeed.

The problem can be compared to [CVE-2021-33840](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-33840) but goes much further.

Also the application does not contain any encryption in frontend/backend parts (apart from transport encryption via https). The backend gets unencrypted json data and delivers unenecrypted json data.

## Exploit code
The fully automated exploit code in Perl language creates 500 users (from random name databases) with random but valid looking phone numbers and all of the users are checked in into one room. Fully automated. A time delay of 5s/request was inserted to not overload the server and to not trigger any protection that might have existed.
The exploit ran fully automated successfully and I reported the problem including the created fake users to the university on that same day.

Before choosing a room I made sure the room was not visited on that day. This is possible as the application contains a counter with how many people one was in contact with in a specific room. So if one visits a room and that counter is 0 no one else was in that room during that time frame.

## Reporting
The application was also affected by a RCE from [CVE-2021-3129](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-3129) which I reported on 12.04.2021 to the university.

The current problem was reported on 31.05.2021.

For both reports I did receive a confirmation of receipt email the day after I reported them. I did not receive anything else afterwords.

A [request to the source code](https://fragdenstaat.de/anfrage/quellcode-digitale-kontaktnachverfolgung/) of the application is in progress.
