$(document).ready(function () {
    writeToStatusPanel("Welcome!!!\n");
    createDropdownMenu();
    populateFileView();
    populateCardView("testCard.vcf");
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

                    var newNumText = document.createTextNode(data[i].filename);
                    var newNumProp = document.createTextNode(data[i].name);
                    var newNumPropValue = document.createTextNode(data[i].numprops);

                    var downloadLink = document.createElement('a');
                    downloadLink.appendChild(newNumText);
                    downloadLink.setAttribute('href','/uploads/'+data[i].filename);
                    
                    COL1.appendChild(downloadLink);
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
        type: 'GET',
        url: '/getVCardSummaries',
        success: function (data) {
            try{
                for(i = 0; i < data.length; i++){
                    var newMenuItem = document.createElement('button');
                    newMenuItem.setAttribute('id',data[i].filename);
                    newMenuItem.setAttribute('class','btn btn-link');
                    newMenuItem.innerHTML = data[i].filename;
                    newMenuItem.addEventListener('click',function(){
                        populateCardView(data[i].filename).bind(data[i].filename);
                    });
                    $("#card_view_menu").append(newMenuItem);
                }
            }catch(e){
                writeToStatusPanel("Error with menu.\n");
            }
        }, fail:function () {
            alert("Issue");
        }
    });
}
function writeToStatusPanel(toBeWritten) {
    $('#status_output_p').append(toBeWritten);
}
$('#clearBTN').click(function(){
    $('#status_output_p').remove();
});