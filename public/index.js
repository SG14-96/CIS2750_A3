//upload new file to the server
$('#uploadFileNow').on('click', function () {
    var newData = new FormData($('#textfile'));

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

//get card data from server
function populateFileView() {
    var filetable = document.getElementById('filelogview_items');
    var dataFromServer;
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

//download card file from server
function getCardFile() {
    //
}