$(document).ready(function () {
    // load data from file.
});
//upload new file to the server
$('#uploadFileNow').on('click', function () {
    let newData = new FormData($('#textfile'));

    $.ajax({
        type: "POST",
        url: "url",//change to ip for Node server
        data: newData,
        success: function (data) {
            $('#output').html(data);
        },
        cache:false,
        contentType:false,
        processData:false,
    });
});

function populateFileView() {
    let vcardDataJSON;
    $.ajax({
        type: "GET",
        url: "url",
        data: "String",
        dataType: "json",
        success: function (data) {
            vcardDataJSON = data;
        }
    });
}
//get card data from server
function populateFileView() {
    let filetable = document.getElementById('filelogview_items');
    let dataFromServer;
    $.ajax({
        url: "url", // replace this with correct url
        data: "String",
        dataType: "json",
        success: function (data) {
            dataFromServer = data;
        }
    });

    for(card in dataFromServer){
        console.log(card);
    }
}

function writeToStatusPanel() {
    //
}
//download card file from server
function donwloadFile() {
    //
}