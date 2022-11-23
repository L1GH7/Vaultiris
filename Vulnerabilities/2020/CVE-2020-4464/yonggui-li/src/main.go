package main

import (
	"encoding/json"
	"fmt"
	"goland1/src/spider/NVDInfoField"
)

// 程序执行入口
func main() {
	cveDemo1 := NVDInfoField.NVDInfo{
		SeverityInfo: NVDInfoField.Severity{
			CveID: "CVE-2020-4450",
			Cvs2:  NVDInfoField.CVS2{},
			Cvs3: NVDInfoField.CVS3{
				Score3Nvd: 7.8,
				Level:     NVDInfoField.High,
				VectorNvd: "AV:N\\AC:L",
			},
		},
		CweInfo: []NVDInfoField.CWE{
			{
				CweNum:  "CWE-67",
				CweName: "remote code exec",
			},
		},
	}

	fmt.Println(cveDemo1)
	fmt.Println("*****************")

	ser, err := json.MarshalIndent(cveDemo1, "", "  ")
	if err != nil {
		panic(err.Error())
	}
	//fmt.Println(ser) // 二进制json
	fmt.Println(string(ser))

	jsonStr := "{\"SeverityInfo\":{\"CveID\":\"CVE-2020-4450\",\"Cvs2\":{\"Score2Nvd\":0,\"Level\":0,\"Vector\":\"\"},\"Cvs3\":{\"Score3Nvd\":7.8,\"Score3Cna\":0,\"Level\":2,\"VectorNvd\":\"AV:N\\\\AC:L\",\"VectorCna\":\"\"}},\"CweInfo\":[{\"CweNum\":\"CWE-67\",\"CweName\":\"remote code exec\",\"CweLink\":\"\"}]}"
	var nvdInfoDemo NVDInfoField.NVDInfo
	//var nvdInfoDemo struct{ Any interface{} }
	if err := json.Unmarshal([]byte(jsonStr), &nvdInfoDemo); err != nil {
		panic(err.Error())
	}

	//	c := colly.NewCollector(
	//		colly.Async(false),
	//	)
	//	url := "https://nvd.nist.gov/vuln/detail/CVE-2020-4464"
	//	cwe := new(NVDInfoField.CWE)
	//	res, c := cwe.GetCWE(url, c)
	//	fmt.Println(res.CweNum)
	//
	//	//c1 := colly.NewCollector()
	//	cvs := new(NVDInfoField.CVS3)
	//	res1, c := cvs.GetCVS3(url, c)
	//
	//	c.Visit(url)
	//	fmt.Println(res1.Score3Nvd, res1.Score3Cna)
}
