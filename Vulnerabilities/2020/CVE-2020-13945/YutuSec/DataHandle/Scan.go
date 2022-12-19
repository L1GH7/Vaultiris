package DataHandle

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func Scan() {
	ch := GETURLBase()
	for n := 0; n < Thread; n++ {
		for v := range ch {
			wg.Add(2)
			fmt.Printf("正在探测APISIX [-%v-] 未授权访问漏洞\n", v)
			go func(v string) {
				CheckAPISIX_Unauth(v, &wg)
				CheckDefaultkey(v, &wg)
			}(v)
		}
	}
	wg.Wait()
}
