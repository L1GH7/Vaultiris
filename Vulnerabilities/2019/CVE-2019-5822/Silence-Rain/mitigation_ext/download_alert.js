for (let frame of $("iframe")) {
	frame.onload = () => {
		for (let item of frame.contentWindow.document.getElementsByTagName("a")) {
			console.log(item)
			if (typeof($(item).attr("download")) !== undefined) {
				item.addEventListener("click", e => {
					let download_link = /^(https?:\/)?\/[\w-.]+(:\d+)?/i.exec($(item).attr("href"))[0]
					let current_origin = /^https?:\/\/[\w-.]+(:\d+)?/i.exec(document.URL)[0]
					if (download_link !== current_origin) {
						e.preventDefault()
						console.log("cross origin")
						alert("Cross origin download is triggered")
					} else {
						console.log("same origin")
					}
				})
			}
		}
	}
}