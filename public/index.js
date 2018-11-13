$(document).ready(function () {
    populateFileView();
    populateCardView();
});
//upload new file to the server
$('#uploadFileNow').on('click', function (e) {
    e.preventDefault();
    var form = e.target;
    var newData = new FormData(form);
    $.post({
        type: 'POST',
        url: 'http://localhost:8080/upload',//change to ip for Node server
        data: JSON.stringify(newData),
        dataType: 'json',
        contentType: 'application/json',
        success: function () {
            document.getElementById('status_panel').innerHTML = "Uploaded new file";
        },
        fail: function (){
            alert("Could not upload file.\n");
        },
        cache:false,
        contentType:false,
        processData:false,
    });
});

function populateFileView() {
    let vcardDataJSON;
    $.ajax({
        type: 'GET',
        url: 'http://localhost:8080/getVCardSummaries',
        data: "String",
        dataType: "json",
        success: function (data) {
            vcardDataJSON = data;
            console.log(vcardDataJSON);
            $(".filelogview_items").append(" <tr><th>VCardTest.vcf</th><td>Simon Perrault</td><td>15</td></tr> ");
        }
    });
}
//get card data from server
function populateCardView() {
    let filetable = document.getElementById('filelogview_items');
    let dataFromServer;
    $.ajax({
        url: "http://localhost:8080/getVCardProps", // replace this with correct url
        data: "String",
        dataType: "json",
        success: function (data) {
            dataFromServer = data;
            console.log(dataFromServer);
            $(".cardview_items").append("<tr><th>1</th><td>FN</td><td>Simon Perrault</td></tr>");
        }
    });
}

function writeToStatusPanel(toBeWritten) {
    //
}
//download card file from server
function donwloadFile() {
    //
}