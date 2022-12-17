from redteam import RedTeam
from urllib.parse import urlparse

x = RedTeam()
hits = x.googledork("inurl:owa/auth/logon.aspx")
for hit in hits:
        print("https://"+urlparse(hit).netloc+"/")
