<?php 

require_once('datamodel.php');

// returns data as JSON
function returnJSON($data) {
    header("content-type: application/json");
    print json_encode($data);
    exit;
}

// get request method into $path variable
$method = strtolower($_SERVER["REQUEST_METHOD"]);
if (isset($_SERVER["PATH_INFO"])) {
  $path = $_SERVER["PATH_INFO"];
}
else {
  $path = "";
}

// since path comes in as a/b/c, need to split it apart and make sure it passes basic checks
$pathParts = explode("/", $path);
if (count($pathParts) < 2) {
  $response = array("status" => "FAIL", "msg" => "Incomplete URL");
  returnJSON($response);
}
else if ($pathParts[1] !== "v1" && $pathParts[1] === "items") {
    // allow this error -- Campbell approved
}
else if ($pathParts[1] !== "v1") {
  $response = array("status" => "FAIL", "msg" => "Invalid version");
  returnJSON($response);
}

// get any JSON data
$jsonData = array();
try {
  $rawData = file_get_contents("php://input");
  $jsonData = json_decode($rawData, true);
  if ($rawData !== "" && $jsonData == NULL) {
    $response = array("status" => "FAIL", "msg" => "Invalid or no JSON data");
    returnJSON($response);
  }
}
catch (Exception $e) {};

// check if a GET request
if ($method === "get") {
    getRequest($pathParts);
}
// check if a POST request
else if ($method === "post") {
    postRequest($pathParts, $jsonData);
}
// neither a GET nor a POST
else {
    $response = array("status" => "FAIL", "msg" => "Invalid URL");
    returnJSON($response);
}

// function to handle GET requests
function getRequest($pathParts) {
    // checks for rest.php/v1/items
    if (count($pathParts) == 3 && $pathParts[2] === "items") {
        getListOfItems();
    }
	// checks for rest.php/v1/items/token
    else if (count($pathParts) == 4 && $pathParts[2] === "items") {
	    getItemsUserConsumed($pathParts, 3);
    }
    // checks for rest.php/items/token
    else if (count($pathParts) == 3 && $pathParts[1] === "items") {
        getItemsUserConsumed($pathParts, 2);
    }
    // checks for rest.php/v1/itemsSummary/token
    else if (count($pathParts) == 4 && $pathParts[2] === "itemsSummary") {
        getSummaryOfItems($pathParts, 3);
    }
    else {
        $response = array("status" => "FAIL", "msg" => "Invalid URL");
        returnJSON($response); 
    }
}

// function to handle POST requests
function postRequest($pathParts, $jsonData) {
    // checks for rest.php/v1/user
    if (count($pathParts) == 3 && $pathParts[2] === "user") {
        getToken($jsonData);
    }
    // checks for rest.php/v1/items
    else if (count($pathParts) == 3 && $pathParts[2] === "items") {
        updateItemsConsumed($jsonData);
    }
    else {
        $response = array("status" => "FAIL", "msg" => "Invalid URL");
        returnJSON($response); 
    }
}

// function that returns error message for database failure
function databaseFailure() {
    $response = array("status" => "FAIL", "msg" => "Could not connect to the database");
    returnJSON($response);

}

?>
