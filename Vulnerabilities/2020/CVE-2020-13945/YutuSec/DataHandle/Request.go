package DataHandle

import (
	"bytes"
	"io"
	"io/ioutil"
	"mime/multipart"
	"net/http"
	"net/http/httputil"
)

func RequestHead(Main string, url string, bodys io.Reader, head map[string]string) (*http.Response, string, string) {
	resq, err := http.NewRequest(Main, url, bodys)
	if err != nil {
		return nil, "", ""
	}
	for key, val := range head {
		resq.Header.Add(key, val)
	}
	resqbody, err := httputil.DumpRequest(resq, true)
	if err != nil {
		return nil, "", ""

	}
	resp, err := Client.Do(resq) //排除全局变量引起的问题，排除resq为空引发的问题,排除因err定义的问题
	if err != nil {
		return nil, "", ""
	}
	body, err := ioutil.ReadAll(resp.Body)
	defer resp.Body.Close()
	if err != nil {
		return nil, "", ""
	}
	return resp, string(body), string(resqbody)
}
func RequestHeadUnClose(Main string, url string, bodys io.Reader, head map[string]string) *http.Response {
	resq, err := http.NewRequest(Main, url, bodys)
	if err != nil {
		return nil
	}
	for key, val := range head {
		resq.Header.Add(key, val)
	}
	if err != nil {
		return nil

	}
	resp, err := Client.Do(resq) //排除全局变量引起的问题，排除resq为空引发的问题,排除因err定义的问题
	if err != nil {
		return nil
	}
	return resp
}
func NewfileUploadRequest(uri string, filepara string, filename string, content string, params map[string]string, header map[string]string) (*http.Response, string) {
	body := &bytes.Buffer{}
	writer := multipart.NewWriter(body)
	boundary := writer.Boundary()

	w, _ := writer.CreateFormFile(filepara, filename) //创建文件上传表单
	w.Write([]byte(content))
	if params != nil {
		for key, val := range params {
			_ = writer.WriteField(key, val) //写入其他参数
		}
	}
	writer.Close()
	req, err := http.NewRequest("POST", uri, body)
	req.Header.Add("Content-Type", "multipart/form-data; boundary="+boundary)
	if err != nil {
		return nil, ""
	}
	for k, v := range header {
		req.Header.Add(k, v)
	}
	resqbody, err := httputil.DumpRequest(req, true)
	if err != nil {
		return nil, ""
	}
	resp, err := Client.Do(req)
	if err != nil {
		return nil, ""
	}
	return resp, string(resqbody)
}
