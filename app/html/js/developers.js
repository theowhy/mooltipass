var mooltipass = mooltipass || {};
mooltipass.ui = mooltipass.ui || {};
mooltipass.ui.developers = mooltipass.ui.developers || {};


/**
 * Initialize function
 * triggered by mooltipass.app.init()
 */

mooltipass.ui.developers.init = function () {
    $('#page-developers #resetCardCheckbox').change(function() {
        $('#page-developers button.resetCard').prop('disabled', !$(this).prop('checked'));
    });
    $('#page-developers button.resetCard').click(function() {
        $(this).prop('disabled', true);
        $('#page-developers #resetCardCheckbox').prop('disabled', true).prop('checked', false).data('active', 1);
        mooltipass.device.interface.send({
            'command': 'resetCard',
            'callbackFunction': function(_status) {
                if(_status.status == 1) {
                    mooltipass.ui.status.success($('#page-developers button.resetCard'), 'Factory reset for card successful');
                }
                else {
                    mooltipass.ui.status.error($('#page-developers button.resetCard'), 'Could not do a factory reset for the inserted card');
                }
                $('#page-developers #resetCardCheckbox').prop('disabled', false).data('active', 0);
            },
        });
    });

    $('#page-developers button.import').click(function() {

        $("#modal-confirm-on-device").show();

        mooltipass.device.interface.send({
            'command': 'startSingleCommunicationMode',
            'callbackFunction': mooltipass.ui.sync.callbackImportMediaBundle,
            'reason': 'mediabundlerupload',
            'callbackFunctionStart': function() {
                mooltipass.device.singleCommunicationModeEntered = true;

                var password = $('#importMediaBundlePassword').val();
                mooltipass.memmgmt.mediaBundlerUpload(mooltipass.ui.developers.callbackImportMediaBundle, password);
            }
        });

    });

    $('#page-developers button.jump').click(function() {
        if($('#jumpToBootloaderPassword').val() != 'limpkin') {
            mooltipass.ui.status.error($('#page-developers button.jump'), 'Password wrong');
            return;
        }

        mooltipass.device.interface.send({
            'command': 'startSingleCommunicationMode',
            'callbackFunction': mooltipass.ui.sync.callbackJumpToBootloader,
            'reason': 'jumptobootloader',
            'callbackFunctionStart': function() {
                mooltipass.device.singleCommunicationModeEntered = true;

                setTimeout(function() {
                    mooltipass.device.reset();
                    mooltipass.device.restartProcessingQueue();
                }, 2000);
            }
        });

    });
};

mooltipass.ui.developers.callbackImportMediaBundle = function(_statusObject) {
    if(_statusObject.success) {
        mooltipass.ui.status.success($('#page-developers button.import'), 'Media bundle imported successfully');
    }
    else {
        mooltipass.ui.status.error($('#page-developers button.import'), _statusObject.msg);
    }

    $("#modal-confirm-on-device").hide();

    mooltipass.device.endSingleCommunicationMode(_statusObject.skipEndingSingleCommunicationMode);
};

mooltipass.ui.developers.callbackJumpToBootloader = function(_statusObject) {
    if(_statusObject.success) {
        mooltipass.ui.status.success($('#page-developers button.jump'), 'Successfully jumped to bootloader');
    }
    else {
        mooltipass.ui.status.error($('#page-developers button.jump'), _statusObject.msg);
    }

    $("#modal-confirm-on-device").hide();

    mooltipass.device.endSingleCommunicationMode(_statusObject.skipEndingSingleCommunicationMode);
};
