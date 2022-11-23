<?php
$fh = fopen('php://memory', 'rw');
fwrite($fh, "abc");
rewind($fh);
stream_filter_append($fh, 'convert.iconv.iso-10646/utf8//IGNORE', STREAM_FILTER_READ, []);
echo stream_get_contents($fh);
?>
