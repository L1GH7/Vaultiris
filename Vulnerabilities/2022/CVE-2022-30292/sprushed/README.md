# CVE-2022-30292

## Description
This vulnerability is the heap-based buffer overflow in the function thread_call in Squirrel 3.2 and below. It caused by the lack of the call of sq_reservestack function. This vulnerability leads to DoS of the client and could possibly lead to sandbox escape and arbitrary code execution

## Attack vector
To exploit vulnerability someone must run crafted Squirrel bytecode or source file. This can be accomplished in multiple ways. For example, a server sends the game map to the client and one executes the malicious Squirrel script. The another way of being attacked is installing a malicious game mod onto your server.

## Fix
There is the fix [commit](https://github.com/albertodemichelis/squirrel/commit/a6413aa690e0bdfef648c68693349a7b878fe60d) for this vulnerability. To apply the fix you should recompile the project from sources since this commit.

## Affected applications
There are lots of [games](https://en.wikipedia.org/wiki/Squirrel_(programming_language)#Games_using_Squirrel) which use Squirrel as a main scripting language. Developers of these games must apply the fix as soon as possible.

## Proof of concept
We will not provide the PoC before 90 days from the date the fix was published. Perhaps we will provide it later.

## Credits
- Pavel Blinnikov (pturtle)
- Ilya Titov (KyberGnida)
- Andrey Chizhov (thsage)
- Alexander Zhdanov (hydrag)
