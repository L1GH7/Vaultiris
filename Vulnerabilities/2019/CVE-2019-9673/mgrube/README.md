# CVE-2019-9673: Freenet content filter vulnerability

**NOTE: I have fully disclosed this bug to the Freenet team and worked with them to verify their patch. The patch is now deployed in the latest version of Freenet.**

I've recently found a security vulnerability in Freenet that may allow an attacker to 
de-anonymize a target or send malicious documents through Freenet. 

## Impact
This vulnerability impacts Freenet users who are using Firefox as their browser. It allows de-anonymization and other malicious behavior on a user's computer.  It is present in all versions through 1483.

## Exploit
This exploit works by taking advantage of a disconnect between Firefox and Freenet in terms
of handling MIME types. Freenet ships with filters for many different content types - a large 
amount of effort has been put in to making sure that only well-formed and non-scripted content 
can be rendered without a number of warnings to the user. 

Freenet generally does a good job of handling data the way it should and allowing the user to specify
if they'd like it handled some other way. 

Firefox, on the other hand, is a bit more nuanced in how it determines the MIME type of a file. Specifically
as documented [here](https://developer.mozilla.org/en-US/docs/Mozilla/How_Mozilla_determines_MIME_Types). Most
of the process documented on this page is beyond an attacker's control, especially if they must attack 
through Freenet. One thing that _is_ within an attacker's control, however, is the MIME type of the data 
being inserted. When data is inserted into the Freenet network, a user can specify the MIME type - when 
this field is left null, the data is treated as application/octet-stream and receives all of the proper 
treatment regarding warnings. 

However, navigating to the HTTP section of the Mozilla documentation, we see that Firefox actually makes
its own guesses about the MIME type of the data when certain conditions are not met by the application that
serves the content. Specifically, when the Content-Encoding header is not sent by an application, Firefox
actually sniffs the content to decide what to do with it. If the first block of data is non-text, Firefox
will treat the file as the MIME type that is indicated by the extension. 

It turns out that Freenet was not sending this header in its responses, which allows us to turn this into
something usable. Since our data can be inserted as MIME type text/plain, which obviously does not receive
sophisticated filtering, but can be delivered as whatever MIME type is indicated by the file extension, we
now have a way to deliver an unfiltered HTML document(or PDF, or .docx...). This is a big deal because normally
all dangerous content is recommended for download into a folder or temporary space and opened outside of the 
browser. 



## Demo

This is a quick video of the exploit working - in this case to deliver an unfiltered HTML file. 

[![Freenet Filter Bypass](https://img.youtube.com/vi/yYFkCK0Jt9A/0.jpg)](https://www.youtube.com/watch?v=yYFkCK0Jt9A)

For a working example, install 1483 or earlier and navigate to:

http://127.0.0.1:8888/SSK@Xv~K9QDJQTjy4x8jOO8rfqK42JDliBes4GKS0RLLcdA,dCD8gDm0c1-JwuDAx9ENqZbGWU3BSmje0XOaMoc6iFw,AQACAAE/astley.html  

For anybody playing along at home, you can obtain the most recent vulnerable version of Freenet [here](https://github.com/freenet/fred/releases/download/build01483/new_installer_offline_1483.jar).

## Attack Scenario

So how could we use this in the real world? 

It turns out that if you link to your malicious content within Freenet on a page already being browsed by the user, Freenet will make sure to treat the file as the type indicated by its extension. Freenet will see that our first bit of data is binary and warn the user that our file is malicious. 

:( 

This means that if we want our target to view our malicious page with JS on it, we must link to it externally. This is actually fine, because Freenet's most popular messaging systems do not operate from Freenet's web proxy. 

If, for example, the demo URI in this writeup were linked within FMS, there would be no extra check and we can have our payload run. 

All that would be necessary to de-anonymize a number of Freenet users would be to create content with an interesting title, insert a file with the first block of data as binary and then create a convincing web page that silently reports IP address and activity in the background. Combine this with a toolset like the BeEF framework and you've got some interesting opportunities.

We can, of course, also use this as a vector to deliver a malicious PDF, .docx or other files and gain a more permanent foothold on the user's machine. 

## The Fix

Fixing this bug is quite simple - from now on FProxy will always pass the Content-Encoding HTTP header when it delivers content. This header tells the Firefox browser to treat MIME types explicitly as defined by FProxy. As a result, data of type text/plain will _always_ be rendered as plaintext and not treated as any other MIME type. 

## Conclusion

This bug was, all in all, fairly simple - yet it had the ability to impact the functionality of Freenet in a major way. MIME Type checking is really important and a major lesson to learn from this bug is that MIME types do not always get handled in a consistent way when data is passed from one program to another. 


## TL;DR
Just insert some data into Freenet with MIME type text/plain and the first block containing only binary data. Firefox will treat it as whatever type the file extension specifies and it will be delivered to the user directly instead of being filtered by Freenet. Send to your victims on FMS
or Frost. Collect their IP addresses or get them to run a binary payload. Profit!

