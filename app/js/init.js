var _listenerInstalled = false;
var _forceInitialShow = false;


/* ######################################################################################################### */


function launch(details) {
    // TODO: As of 11/2014 this event is not fired in case of granting new permissions
    //   http://stackoverflow.com/questions/2399389/detect-chrome-extension-first-run-update#comment32831961_14957674
    if(details.reason == "install"){
        //console.log('Install', details);
        //console.log("This is a first install!");
        _forceInitialShow = true;
    }else if(details.reason == "update"){
        //console.log('Update', details);
        //var thisVersion = chrome.runtime.getManifest().version;
        //console.log("Updated from " + details.previousVersion + " to " + thisVersion + "!");
        chrome.storage.local.clear();
        chrome.storage.local.get('changelog', function (result) {
            var currentVersion = chrome.runtime.getManifest().version;
            var storedVersion = (result.changelog) ? result.changelog.version : null;
            if(currentVersion != storedVersion) {
                if(storedVersion == null) {
                    _forceInitialShow = true;
                }
            }
        });
    }

    installListener();
}


function installListener() {
    if(_listenerInstalled) {
        return;
    }

    chrome.runtime.onInstalled.addListener(launch);
    chrome.runtime.onStartup.addListener(launch);
    chrome.app.runtime.onLaunched.addListener(launchWindow);

    // Listen for external messages (e.g. from extension) and send them to the app
    // /vendor/mooltipass/app.js is listening for incoming internal messages
    chrome.runtime.onMessageExternal.addListener(
        function(message, sender, callbackFunction) {
            //console.log('chrome.runtime.onMessageExternal(' + sender.id + '):', message);

            if('show' in message) {
                launchWindow(true);
                return;
            }

            var data = {'id': sender.id, 'message': message};
            // Keep callbackFunction separated to react on chrome.runtime.lastError
            chrome.runtime.sendMessage(data, callbackFunction);
        });

    //console.log('Listener installed');

    _listenerInstalled = true;

    setTimeout(function() {
        launchWindow();
    }, 2000);
}


function launchWindow(forceOpen) {
    // AppWindow is already opened -> do not open another one
    var windows = chrome.app.window.getAll();
    //console.log('Length:', windows.length);

    if(windows.length > 0) {
        _forceInitialShow = false;
        windows[0].show();
        return;
    }

    var options = {'bounds': {'width': 820, 'height': 550}, "resizable": false, "hidden": true};
    if(arguments.length == 1 || _forceInitialShow) {
        _forceInitialShow = false;
        options.hidden = false;
    }
    chrome.app.window.create(
        'html/index.html',
        options,
        function(createdWindow) {
            createdWindow.onClosed.addListener(reopenHiddenWindow);
        }
    );
}

function reopenHiddenWindow() {
    // AppWindow is already opened -> do not open another one
    var windows = chrome.app.window.getAll();

    if(windows.length > 0) {
        setTimeout(reopenHiddenWindow, 100);
        return;
    }

    launchWindow();
    //console.log('launched');
}


/* ######################################################################################################### */


installListener();