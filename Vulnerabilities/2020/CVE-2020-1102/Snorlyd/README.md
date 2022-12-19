# https-nj.gov---CVE-2020-11023
#### Vulnearability Report of the New Jersey official site
Potential XSS vulnerability when appending HTML containing option elements.

Passing HTML containing `<option>` elements from untrusted sources - even after sanitizing them - to one of jQuery's DOM manipulation methods (i.e. `.html()`, `.append()`, and others) may execute untrusted code.

# RECOMMENDATION
This problem is patched in jQuery 3.5.0; Therefore, it would only be necessary to update it.
To fix this bug without updating it, we can use [DOMPurify](https://github.com/cure53/DOMPurify) with its `SAFE_FOR_JQUERY` option to sanitize the HTML string before passing it to a jQuery method.

At least jQuery 1.12/2.2 or later is required to apply this workaround.

# REFERENCES
https://blog.jquery.com/2020/04/10/jquery-3-5-0-released/
#### For more information
If you have any questions or comments about this advisory, search for a relevant issue in the [jQuery repo](https://github.com/jquery/jquery/issues). If you don't find an answer, open a new issue.
