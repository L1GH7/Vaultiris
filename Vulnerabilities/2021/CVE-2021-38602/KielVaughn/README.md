# CVE-2021-38602

A stored cross site scripting vulnerability is present on the Article editing page in version 5.8.7 of PluXML. User input is not properly sanitized in multiple fields.

## Vulnerable Fields:

- Headline (optional):
- Content:

![Create Article Page](PluXML_Create_Article.png)

Once inserted, XSS can be triggered by visiting the posted article at the link mentioned under **Link to article:** near the top of the page.

![Link to Article](PluXML_Link_to_Article.png)

### Headline Stored XSS Example
---

![Headline XSS](PluXML_Headline_Stored_XSS.png)

### Content Stored XSS Example
---

![Content XSS](PluXML_Content_Stored_XSS.png)
