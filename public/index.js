
$(document).ready(function () {
    //createDropdownMenu();
    populateFileView();
    populateCardView("testCard.vcf");
});

var i = 0;
var fileNameTest; 
//upload new file to the server
$('#uploadFileNow').on('click', function (e) {
    e.preventDefault();
    var form = e.target;
    var newData = new FormData(form);
    $.post({
        type: 'POST',
        url: 'http://localhost:30439/upload',//change to ip for Node server
        data: newData,
        success: function () {
            writeToStatusPanel("Uploaded new file succesfully.");
        },
        fail: function (){
            alert("Could not upload file.\n");
        }
    });
});

function populateFileView() {
    let vcardDataJSON;
    $.ajax({
        type: 'GET',
        url: '/getVCardSummaries',
        contentType: 'application/json',
        dataType: 'json',
        success: function (data) {
            try{                
                for(var i = 0; i < data.length; i++){
                    
                    var newTableEntry = document.createElement('tr');
                    var COL1 = document.createElement('td');
                    var COL2 = document.createElement('td');
                    var COL3 = document.createElement('td');
                    

                    var downloadLink = document.createElement('a');
                    downloadLink.setAttribute('href','/uploads/'+data[i].filename);

                    var newNumText = document.createTextNode(data[i].filename);
                    var newNumProp = document.createTextNode(data[i].name);
                    var newNumPropValue = document.createTextNode(data[i].numprops);
                    COL1.append(downloadLink);
                    COL1.appendChild(newNumText);
                    COL2.appendChild(newNumProp);
                    COL3.appendChild(newNumPropValue);
                    
                    newTableEntry.appendChild(COL1);
                    newTableEntry.appendChild(COL2);
                    newTableEntry.appendChild(COL3);

                    $('#file_log_items').append(newTableEntry);
                }
            }catch(e){
                writeToStatusPanel("Error loading vcard information to table.\n");
            }
        }, fail:function () {
            writeToStatusPanel("There was an issue retriven file log view items.\n");
        }
    });
}
//get card data from server

function populateCardView(fileName) {
    let filetable = document.getElementById('filelogview_items');
    let dataFromServer;
    $.ajax({
        url: "getVCardProps/"+fileName, // replace this with correct url
        success: function (data) {
            try{

                var counter = 0;
                for(key in data){

                    var newTableEntry = document.createElement('tr');
                    var COL1 = document.createElement('td');
                    var COL2 = document.createElement('td');
                    var COL3 = document.createElement('td');
                    
                    var newNumText = document.createTextNode(counter);
                    var newNumProp = document.createTextNode(key);
                    var newNumPropValue = document.createTextNode(""+data[key][0].value);
                   
                    COL1.appendChild(newNumText);
                    COL2.appendChild(newNumProp);
                    COL3.appendChild(newNumPropValue);
                    
                    newTableEntry.appendChild(COL1);
                    newTableEntry.appendChild(COL2);
                    newTableEntry.appendChild(COL3);

                    $('#card_view_panel').append(newTableEntry);
                    counter++;
                }
            }catch(e){
                writeToStatusPanel("Error loading vcard information to table.\n");
            }
        }, fail:function () {
            alert("Issue");
        }
    });
}

function createDropdownMenu(){
    $.ajax({
        type: "get",
        url: "/uploads",
        dataType: "application/json",
        data:data,
        success: function (data) {
            vcardDataJSON = JSON.parse(data);
            // console.log(vcardDataJSON);
            try{
                for(i = 0; i < data.length; i++){
                    console.log("try");
                    
                    var dropdownMenu = document.getElementById('#dropMenu');
                    var link = document.createElement('a');
                    link.setAttribute('href', "#");
                    link.appendChild(document.createTextNode(vcardDataJSON[i].filename));
                    var thisLI = document.createElement('li');
                    thisLI.appendChild(link); 
                    dropdownMenu.append(thisLI);
                }
            }catch(e){
                //
            }
        }, fail:function () {
            alert("Issue");
        }
    });
}

function writeToStatusPanel(toBeWritten) {
    $('#status_output_p').text(toBeWritten);
}