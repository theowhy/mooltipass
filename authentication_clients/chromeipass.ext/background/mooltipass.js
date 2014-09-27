
var mooltipass = {};
var mpClient = null;
var contentAddr = null;
var connected = null;
var mpInputCallback = null;
var mpUpdateCallback = null;



var maxServiceSize = 123;       // Maximum size of a site / service name, not including null terminator

function mpCheckConnection()
{
    if (!connected) {
        if (!mpClient) {
            // Search for the Mooltipass Client
            chrome.management.getAll(getAll);
        } else {
            chrome.runtime.sendMessage(mpClient.id, { type: 'ping' });
            setTimeout(mpCheckConnection,500);
        }
    }
}

function getAll(ext)
{
    for (var ind=0; ind<ext.length; ind++) {
        if (ext[ind].shortName == 'Mooltipass Client') {
            mpClient = ext[ind];
            break;
        }
    }

    if (mpClient != null) {
        chrome.runtime.sendMessage(mpClient.id, { type: 'ping' });
        console.log('found mooltipass client "'+ext[ind].shortName+'" id='+ext[ind].id);
    } else {
        console.log('No mooltipass client found');
    }

    setTimeout(mpCheckConnection,500);
}

// Search for the Mooltipass Client
chrome.management.getAll(getAll);

// Messages from the mooltipass client app
chrome.runtime.onMessageExternal.addListener(function(request, sender, sendResponse) 
{
    console.log('back: app req '+JSON.stringify(request));
    switch (request.type) {
        case 'credentials':
            console.log('back: got credentials '+JSON.stringify(request));
            //chrome.tabs.sendMessage(contentAddr, request);
            if (mpInputCallback) {
                mpInputCallback([{Login: request.inputs.login.value, Name: '<name>', Uuid: '<Uuid>', Password: request.inputs.password.value, StringFields: []}]);
                mpInputCallback = null;
            }
            break;
        case 'updateComplete':
            console.log('back: got updateComplete');
            if (mpUpdateCallback) {
                mpUpdateCallback('success');
                mpUpdateCallback = null;
            }
            //chrome.tabs.sendMessage(contentAddr, request);
            break;
        case 'connected':
            console.log('back: got connected');
            connected = request;
            //if (contentAddr) {
                //chrome.tabs.sendMessage(contentAddr, request);
            //}
            break;
        case 'disconnected':
            console.log('back: got disconnected');
            connected = null;
            //if (contentAddr) {
                //chrome.tabs.sendMessage(contentAddr, request);
            //}
            break;
        case 'cardPresent':
            console.log('back: got cardPresent');
            //if (contentAddr) {
                //chrome.tabs.sendMessage(contentAddr, request);
            //}
            //if (!request.state){
                //chrome.browserAction.setIcon({path: 'mooltipass-nocard.png'});
            //}
            break;
        case 'rescan':
            console.log('back: got rescan');
            //if (contentAddr) {
                //chrome.tabs.sendMessage(contentAddr, request);
            //}
            break;
        default:
            break;
    }
});

mooltipass.addCredentials = function(callback, tab, username, password, url) 
{
    mooltipass.associate();
    mooltipass.updateCredentials(callback, tab, null, username, password, url);
}

mooltipass.isConnected = function()
{
    return connected != null;
}

// needs to block until a response is received.
mooltipass.updateCredentials = function(callback, tab, entryId, username, password, url) 
{
    mooltipass.associate();
	console.log("mp.updateCredentials(})", tab.id, entryId, username, url);

	// unset error message
	page.tabs[tab.id].errorMessage = null;

    chrome.runtime.sendMessage({type: 'update', url: url, inputs: {login: {id: 0, name: 0, value: username}, password: { id: 1, name: 1, value: password }}});

    request = { type: 'update',
                url: url, 
                inputs: {
                    login: {id: 'login.id', name: 'login.name', value: username},
                    password: {id: 'pass.id', name: 'pass.name', value: password} } };

    console.log('sending update to '+mpClient.id);
    contentAddr = tab.id;
    mpUpdateCallback = callback;
    chrome.runtime.sendMessage(mpClient.id, request);

    // this needs to be blocking, but can't because we're waiting on an async response from the mp app and the mp, which may never arrive.
    // So this actually needs to tight loop until an mp response arrives, with a timeout.
    if (false) {
	var result = keepass.send(request);
	var status = result[0];
	var response = result[1];

	// verify response
	var code = "error";
	if(keepass.checkStatus(status, tab)) {
		var r = JSON.parse(response);
		if (keepass.verifyResponse(r, key, id)) {
			code = "success";
		}
		else {
			code = "error";
		}
	}
    }

	//callback("success");
}


mooltipass.associate = function(callback, tab) 
{
    if (!mpClient) {
        console.log('mp.associate()');
        chrome.management.getAll(getAll);
    } else if (!connected) {
        // try pinging the app
        chrome.runtime.sendMessage(mpClient.id, { type: 'ping' });
        console.log('mp.associate() already have client connection, sending ping');
    } else {
        console.log('mp.associate() already connected');
    }
}


mooltipass.generatePassword = function(callback, tab) 
{
    console.log('mp.generatePassword()');
}

mooltipass.copyPassword = function(callback, tab)
{
    console.log('mp.copyPassword()');
}


mooltipass.retrieveCredentials = function(callback, tab, url, submiturl, forceCallback, triggerUnlock) 
{
    mooltipass.associate();
	page.debug("mp.retrieveCredentials(callback, {1}, {2}, {3}, {4})", tab.id, url, submiturl, forceCallback);

	// unset error message
	page.tabs[tab.id].errorMessage = null;

	// is browser associated to keepass?
	if (!mooltipass.isConnected()) {
		browserAction.showDefault(null, tab);
		if(forceCallback) {
			callback([]);
		}
		return;
	}

    request = { type: 'inputs',
                url: submiturl, 
                inputs: {
                    login: {id: 'login.id', name: 'login.name'},
                    password: {id: 'pass.id', name: 'pass.name'} } };

    console.log('sending to '+mpClient.id);
    contentAddr = tab.id;
    mpInputCallback = callback;
    chrome.runtime.sendMessage(mpClient.id, request);
}
