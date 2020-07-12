var token;

// reaches getToken() in datamodel.php
function loginUser(formData) {
  $.ajax({
    type: "POST",
    url: "rest.php/v1/user",
    data: JSON.stringify(formData),
    dataType: "json",
    contentType: "application/json",
    success: function(response) {
      if (response["status"] == "OK") {
        token = response["token"];
        populateButtons();
        populateNewEntriesTable();
        populateViewEntriesTable();
        $("#unauthenticated").css("display", "none");
        $("#authenticated").css("display", "block");
      } 
      else {
        alert(response["msg"]);
      }
    },
    error: function(err) {
      console.log(err);
    }
  });
}

// reaches getListOfItems() in datamodel.php
function populateButtons() {
  $.ajax({
    type: "GET",
    url: "rest.php/v1/items",
    dataType: "json",
    contentType: "application/json",
    success: function(response) {
      if (response["status"] == "OK") {
        var items = response["items"];
        $.each(items, function(key, value) {
          var pk = value["pk"];
          var item = value["item"];
          createButtons(pk, item);
        });
      } 
      else {
        alert(response["msg"]);
      }
    },
    error: function(err) {
      console.log(err);
    }
  });
}

function createButtons(pk, item) {
  var button = '<li class="food-buttons"><button type="button" pk=' + pk + ' onclick="entryAdd(this)">' + item + '</button></li>';
  $("#new-entry--buttons").append(button);
}

// reaches getItemsUserConsumed() in datamodel.php
function populateViewEntriesTable() {
  $.ajax({
    type: "GET",
    url: "rest.php/v1/items/" + token,
    dataType: "json",
    contentType: "application/json",
    success: function(response) {
      if (response["status"] == "OK") {
        $("#view-entries--table-body").html("");
        var items = response["items"];
        var i = 0;
        while (i < 30) {
          var entry = items[i];
          entry = JSON.stringify(entry);
          entry = JSON.parse(entry);
          var item = entry["item"];
          var time = entry["timestamp"];
          createViewEntriesTable(item, time);
          i++;
        }
      } 
      else {
        alert(response["msg"]);
      }
    },
    error: function(err) {
      console.log(err);
    }
  });
}

function createViewEntriesTable(item, time) {
  var row = '<tr><td>' + item + '</td><td>' + time + '</td></tr>';
  $("#view-entries--table-body").append(row);

}

// reaches getSummaryOfItems() in datamodel.php
function populateNewEntriesTable() {
  $.ajax({
    type: "GET",
    url: "rest.php/v1/itemsSummary/" + token,
    dataType: "json",
    contentType: "application/json",
    success: function(response) {
      if (response["status"] == "OK") {
        $("#new-entry--table-body").html("");
        var items = response["items"];
        $.each(items, function(key, value) {
          var item = value["item"];
          var count = value["count"];
          createNewEntriesTable(item, count);
        });
      } 
      else {
        alert(response["msg"]);
      }
    },
    error: function(err) {
      console.log(err);
    }
  });
}

function createNewEntriesTable(item, count) {
  var row = '<tr><td>' + item + '</td><td>' + count + '</td></tr>';
  $("#new-entry--table-body").append(row);

}

// reaches updateItemsConsumed() in datamodel.php
function entryAdd(thisButton) {
  var userToken = token;
  var thisPk = $(thisButton).attr("pk");
  var formData = {token:userToken, itemFK:thisPk}
  $.ajax({
    type: "POST",
    url: "rest.php/v1/items",
    data: JSON.stringify(formData),
    dataType: "json",
    contentType: "application/json",
    success: function(response) {
      if (response["status"] == "OK") {
        populateNewEntriesTable(token);
        populateViewEntriesTable(token);
      } 
      else {
        alert(response["msg"]);
      }
    },
    error: function(err) {
      console.log(err);
    }
  });
}

$(document).ready(function() {
  // event handler for when the login form is submitted
  $("#login-form").submit(function(e) {
    e.preventDefault();
    var form = $("#login-form").serializeArray();
    var formData = {username:form[0].value, password:form[1].value};
    loginUser(formData);
  });
}); //end document ready
