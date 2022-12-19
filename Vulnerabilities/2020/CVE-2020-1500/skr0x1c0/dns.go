package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"strconv"
	"time"
)

const (
	DnsSubdomainLength int = 12
)

type assignSubdomainRequest struct {
	Domain    string `json:"domain"`
	Ttl       uint32 `json:"ttl"`
	ReplaceOk bool   `json:"replaceOk"`
}

func AssignDnsSubdomain(name string) error {
	request := assignSubdomainRequest{
		Domain:    name,
		Ttl:       0,
		ReplaceOk: false,
	}

	body, err := json.Marshal(request)
	if err != nil {
		return fmt.Errorf("cannot marshal assign subdomain request, error: %v", err)
	}

	resp, err := http.Post(
		"http://api.pointer.pw/v1/ssrf/assign", "application/json", bytes.NewBuffer(body))

	if err != nil {
		return fmt.Errorf("cannot execute assign subdomain POST request, error: %v", err)
	}
	if resp.StatusCode == 200 {
		return nil
	}
	body, err = ioutil.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("cannot read error response body")
	}

	return fmt.Errorf("cannot assign domain, error: %s", string(body))
}

type releaseSubdomainRequest struct {
	Domain string `json:"domain"`
}

func ReleaseDnsSubdomain(name string) error {
	request := releaseSubdomainRequest{
		Domain: name,
	}

	body, err := json.Marshal(request)
	if err != nil {
		return fmt.Errorf("cannot marshal release subdomain request, error %v", err)
	}

	resp, err := http.Post(
		"http://api.pointer.pw/v1/ssrf/release", "application/json", bytes.NewBuffer(body))
	if err != nil {
		return fmt.Errorf("cannot execute release subdomain request, error %v", err)
	}
	if resp.StatusCode == 200 {
		return nil
	}

	body, err = ioutil.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("cannot read error response body")
	}
	return fmt.Errorf("cannot release subdomain, error: %s", string(body))
}

type DnsQueryLogEntry struct {
	QType     uint16
	Timestamp time.Time
	Rcode     int
}

func GetDnsQueryLog(name string) ([]DnsQueryLogEntry, error) {
	resp, err := http.Get("http://api.pointer.pw/v1/ssrf/getLog/" + name)
	if err != nil {
		return nil, fmt.Errorf("cannot execute query log get request, error %+v", err)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("cannot read response body, error %+v", err)
	}

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("server returned error, %s", string(body))
	}

	data := struct {
		Log []struct {
			QType     uint16 `json:"qType"`
			Timestamp string `json:"timestamp"`
			Rcode     int    `json:"rCode"`
		} `json:"log"`
	}{}
	if err := json.Unmarshal(body, &data); err != nil {
		return nil, fmt.Errorf("cannot decode json, error %+v", err)
	}

	result := make([]DnsQueryLogEntry, len(data.Log))
	for idx, v := range data.Log {
		result[idx].QType = v.QType
		result[idx].Rcode = v.Rcode
		timestamp, err := strconv.ParseInt(v.Timestamp, 10, 64)
		if err != nil {
			return nil, fmt.Errorf("cannot decode timestamp, error %+v", err)
		}
		result[idx].Timestamp = time.Unix(timestamp, 0)
	}

	return result, nil
}

func GetLatestDnsQueryLogEntry(entries []DnsQueryLogEntry) *DnsQueryLogEntry {
	if len(entries) == 0 {
		return nil
	}

	latest := entries[0]

	for i := 1; i < len(entries); i++ {
		if latest.Timestamp.Before(entries[i].Timestamp) {
			latest = entries[i]
		}
	}

	return &latest
}

func BuildDnsHost(name string) string {
	return name + ".dns.pointer.pw"
}
