package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/http/cookiejar"
	"net/url"
	"strings"
)

var (
	ErrorInvalidSession = fmt.Errorf("session invalid/expired")
)

type XCConfig struct {
	Root     string
	Username string
	Password string
}

type XCClient struct {
	config XCConfig
	client *http.Client
	store  XCStore
}

func NewXCClient(config XCConfig) *XCClient {
	jar, err := cookiejar.New(nil)
	AssertOk(err)
	return &XCClient{
		config: config,
		client: &http.Client{Jar: jar},
		store:  NewInMemoryXCStore(),
	}
}

func (x *XCClient) TriggerNegativeCache(name string) error {
	reqUrl := "http://" + name + "/image.png"
	_, err := x.DocAddFile(reqUrl)

	if err == ErrorInvalidSession {
		if err := x.Login(); err != nil {
			return err
		}
		_, err = x.DocAddFile(reqUrl)
	}

	if err == nil {
		return fmt.Errorf("negative cache trigger expected error, got nothing")
	}

	if strings.Contains(err.Error(), "GENERAL_ARGUMENTS_ERROR") {
		return nil
	}

	return err
}

func (x *XCClient) Login() error {
	form := url.Values{}
	form.Set("action", "login")
	form.Set("name", x.config.Username)
	form.Set("password", x.config.Password)
	form.Set("staySignedIn", "true")

	resp, err := x.client.Post(x.config.Root+"/appsuite/api/login",
		"application/x-www-form-urlencoded", bytes.NewBuffer([]byte(form.Encode())))
	if err != nil {
		return fmt.Errorf("cannot load cookies, error %v", err)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("cannot read response body, error %v", err)
	}

	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("cannot login, status %s, error %s", resp.Status, string(body))
	}

	data := XCLoginResult{}
	if err := json.Unmarshal(body, &data); err != nil || data.Session == "" {
		return fmt.Errorf("login failed, error %s", string(body))
	}
	x.store.SetSession(data)
	return nil
}

type DocAddFileResponse struct {
	FileName string `json:"added_filename"`
	FileId   string `json:"added_fileid"`
}

func (x *XCClient) DocAddFile(imageUrl string) (*DocAddFileResponse, error) {
	form := url.Values{}
	form.Set("action", "addfile")
	form.Set("requestdata", "{\"add_imageurl\":\""+imageUrl+"\"}")
	form.Set("version", "1")
	form.Set("filename", "unnamed.docx")
	form.Set("app", "text")

	session, err := x.store.GetSession()
	if err != nil {
		return nil, err
	}

	resp, err := x.client.Post(
		x.config.Root+"/appsuite/api/oxodocumentfilter?session="+session.Session,
		"application/x-www-form-urlencoded", bytes.NewBuffer([]byte(form.Encode())))
	if err != nil {
		return nil, fmt.Errorf("cannot execute addfile request, error %v", err)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("cannot read response body, error %v", err)
	}

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("addfile request failed, status: %s, error: %s", resp.Status, string(body))
	}

	if strings.Contains(string(body), "added_filename") {
		responseData := struct {
			Data DocAddFileResponse `json:"data"`
		}{}

		if err := json.Unmarshal(body, &responseData); err != nil {
			return nil, fmt.Errorf("cannot unmarshal response data, error %+v", err)
		}

		return &responseData.Data, nil
	}

	if strings.Contains(string(body), "Your session expired") {
		return nil, ErrorInvalidSession
	}
	return nil, fmt.Errorf("add failed, error %s", string(body))
}

func (x *XCClient) Logout() error {
	form := url.Values{}
	form.Set("action", "logout")

	session, err := x.store.GetSession()
	if err != nil {
		return err
	}

	resp, err := x.client.Post(x.config.Root+"/appsuite/api/login?session="+session.Session,
		"application/x-www-form-urlencoded", bytes.NewBuffer([]byte(form.Encode())))
	if err != nil {
		return fmt.Errorf("cannot logout, error %v", err)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("cannot read response body, error %v", err)
	}

	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("cannot logout, status %s, error %s", resp.Status, string(body))
	}

	return nil
}
