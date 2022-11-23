#### This is an updated version of [get-pixels](https://github.com/scijs/get-pixels) which patches the [CVE-2020-8175](https://github.com/advisories/GHSA-w7q9-p3jq-fmhm) security issue. At the time of creation, every file is the same as the ones from the original repository, excluding the package.json.

# get-pixels-updated
##### The following is from the [get-pixels](https://github.com/scijs/get-pixels) GitHub page. As mentioned before, this repository has no breaking changes and is nearly the exact same as the original repository.

Given a URL/path, grab all the pixels in an image and return the result as an [ndarray](https://github.com/mikolalysenko/ndarray).  Written in 100% JavaScript, works both in browserify and in node.js and has no external native dependencies.

Currently the following file formats are supported:

* `PNG`
* `JPEG`
* `GIF`

Example
=======

```javascript
var getPixels = require("get-pixels-updated")

getPixels("lena.png", function(err, pixels) {
  if(err) {
    console.log("Bad image path")
    return
  }
  console.log("got pixels", pixels.shape.slice())
})
```

Install
=======

    npm install get-pixels-updated

### `require("get-pixels-updated")(url[, type], cb(err, pixels))`
Reads all the pixels from url into an ndarray.

* `url` is the path to the file.  It can be a relative path, an http url, a data url, or an [in-memory Buffer](http://nodejs.org/api/buffer.html).
* `type` is an optional mime type for the image (required when using a Buffer)
* `cb(err, pixels)` is a callback which gets triggered once the image is loaded.

**Returns** An ndarray of pixels in raster order having shape equal to `[width, height, channels]`.

**Note** For animated GIFs, a 4D array is returned with shape `[numFrames, width, height, 4]`, where each frame is a slice of the final array.

Credits
=======
Original code from [get-pixels](https://github.com/scijs/get-pixels), updated by [sysollie](https://github.com/sysollie) to fix the [CVE-2020-8175](https://github.com/advisories/GHSA-w7q9-p3jq-fmhm) security issue. Code used and relicensed under and in accordance with the MIT license ([original](https://github.com/scijs/get-pixels/blob/master/LICENSE) | [new](https://github.com/sysollie/get-pixels-updated/blob/main/LICENSE)).
