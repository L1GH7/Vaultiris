# CVE-2019-9653

## 日期
* Disclosure: 03/11/2019
* Last updated: 03/15/2019

## 摘要
NUUO Inc.是一家提供視訊監控解決方案的公司。他們有許多NVR（網路視訊錄影機）產品，可滿足不同客戶的各種要求。這些NVR為Linux的嵌入式視訊錄製系統，可以管理多個攝影機。目前，它們在全世界被許多公共機構、公司、銀行及個人等使用。這些NVR系統的Web界面包含許多嚴重漏洞可遭未經身份驗證的攻擊者利用。我們從中發現了一些易受攻擊的PHP腳本缺乏認證機制和輸入保護，因此它們可在NUUO設備上被利用以root權限執行遠端程式碼。

## 影響範圍
韌體版本 1.7.x 至 3.3.x 版本。

## 建議
更新設備至較新版本，目前最新韌體版本為3.10.x。

## 技術細節
測試目標機器版本為2.3.x的版本，下圖為系統版本資訊。

![image](https://github.com/grayoneday/CVE-2019-9653/blob/master/pic/systeminfo.png)

針對弱點部份修改參數後轉送至目標並且關閉攔截，可以在歷史訊息中發現目標機器回應了執行的指令結果，參照下圖。

![image](https://github.com/grayoneday/CVE-2019-9653/blob/master/pic/PoC1.PNG)

確認執行權限。

![image](https://github.com/grayoneday/CVE-2019-9653/blob/master/pic/PoC2.PNG)

## 發現者
* GrayOD (grayoneday@gmail.com)
* Endpoint Security Team @ NCCST (https://www.nccst.nat.gov.tw/)

## 致謝
* Alan Chung (https://twitter.com/Se7en_5_Sec)
* Endpoint Security Team @ NCCST (https://www.nccst.nat.gov.tw/)
---
