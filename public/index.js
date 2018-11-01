//upload new file to the server
$('#uploadFileNow').on('click', function () {
    var newData = new FormData($('#textfile'));
    //console.log(newData);
    
    /*
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
    */
});

//get card data from server
