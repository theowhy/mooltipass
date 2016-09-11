function status_response(r) {
	$('#initial-state').hide();
	$('#error-encountered').hide();
	$('#need-reconfigure').hide();
	$('#not-configured').hide();
	$('#configured-and-associated').hide();
	$('#configured-not-associated').hide();

	$('#device-connected').hide();
	$('#device-disconnected').show();

	if(!r.keePassHttpAvailable || r.databaseClosed) {
		$('#error-message').html('1: "'+r.error+'"');
		$('#error-encountered').show();
	}
	else if(!r.configured) {
		$('#not-configured').show();
	}
	else if(r.encryptionKeyUnrecognized) {
		$('#need-reconfigure').show();
		$('#need-reconfigure-message').html(r.error);
	}
	else if(!r.associated) {
		//$('#configured-not-associated').show();
		//$('#unassociated-identifier').html(r.identifier);
		$('#need-reconfigure').show();
		$('#need-reconfigure-message').html(r.error);

	}
	else if(typeof(r.error) != "undefined") {
		$('#error-encountered').show();
		$('#error-message').html('2: "'+r.error+'"');
	}
	else {
		$('#device-connected').show();
		$('#device-disconnected').hide();
	}
}

$(function() {
	$("#connect-button").click(function() {
		chrome.runtime.sendMessage({
			action: "associate"
		});
		close();
	});

	$("#reconnect-button").click(function() {
		chrome.runtime.sendMessage({
			action: "associate"
		});
		close();
	});

	$("#reload-status-button").click(function() {
		chrome.runtime.sendMessage({
			action: "get_status"
		}, status_response);
	});

	$("#redetect-fields-button").click(function() {
		chrome.tabs.query({"active": true, "windowId": chrome.windows.WINDOW_ID_CURRENT}, function(tabs) {
			if (tabs.length === 0)
				return; // For example: only the background devtools or a popup are opened
			var tab = tabs[0];

			chrome.tabs.sendMessage(tab.id, {
				action: "redetect_fields"
			});
		});
	});
	
	// Temp patch!!!
		chrome.tabs.query({"active": true, "windowId": chrome.windows.WINDOW_ID_CURRENT}, function(tabs) {
			if (tabs.length === 0)
				return; // For example: only the background devtools or a popup are opened
			var tab = tabs[0];

			chrome.tabs.sendMessage(tab.id, {
				action: "redetect_fields"
			});
		});

	chrome.runtime.sendMessage({
		action: "get_status"
	}, status_response);
});
