# Axios Regular Expression Denial Of Service Attack

This repo hold a POC of [CVE-2021-3749](https://www.cve.org/CVERecord?id=CVE-2021-3749).

## Overview

A ReDoS (regular expression denial of service) flaw was found in the axios package. An attacker that is able to provide crafted input to the trim function may cause an application to consume an excessive amount of CPU.

- Fix Commit: https://github.com/axios/axios/commit/5b457116e31db0e88fede6c428e969e87f290929
