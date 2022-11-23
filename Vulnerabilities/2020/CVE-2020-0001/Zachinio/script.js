Java.perform(function () {

    var ams = Java.use('com.android.server.am.ActivityManagerService');
    ams.getProcessRecordLocked.implementation = function (processName, uid, keepIflarge) {
        var processRecord = this.getProcessRecordLocked(processName, uid, keepIflarge);
        if (uid == 1000) {
            console.log("--------------- getProcessRecordLocked --------------------");
            console.log(processName, ",", uid, ",", keepIflarge);
            if (processRecord) {
                console.log(processRecord, processRecord.pid.value);
                console.log(Java.use("android.util.Log").getStackTraceString(Java.use("java.lang.Exception").$new()))
            }

        }
        return processRecord;
    };

    ams.getContentProvider = function (caller, name, userId, stable) {
        // var contentProvider = this.getContentProvider(caller, name, userId, stable);

        // console.log("--------------- getContentProvider --------------------");
        // console.log(caller, name,userId,stable);
        // return contentProvider;
    };

    var BroadcastQueue = Java.use('com.android.server.am.BroadcastQueue');

    BroadcastQueue.processCurBroadcastLocked = function (broadcastRecord, processRecord, skip) {
        // var contentProvider = this.processCurBroadcastLocked(broadcastRecord, processRecord, skip);

        // console.log("--------------- processCurBroadcastLocked --------------------");
        // console.log( broadcastRecord,processRecord,skip);
        // return contentProvider;
    };

    // var ActiveServices = Java.use('com.android.server.am.ActiveServices');
    // ActiveServices.startServiceLocked.implementation = function (caller, service, resolvedType,
    //     callingPid, callingUid, fgRequired, callingPackage, userId) {

    //     if (userId == 1000 || callingUid == 1000) {
    //         console.log("--------------- startServiceLocked --------------------");
    //         console.log(callingUid, callingPid, callingPackage, service, caller);
    //     }

    //     return this.startServiceLocked(caller, service, resolvedType,
    //         callingPid, callingUid, fgRequired, callingPackage, userId);
    // };

});
