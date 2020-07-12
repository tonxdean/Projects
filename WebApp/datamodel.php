<?php

function getListOfItems() {
    $dbconnection = new mysqli("localhost", "cse383", "HoABBHrBfXgVwMSz", "cse383");
    if ($dbconnection->connect_errno) {
        databaseFailure();
    }
    else {
        $query = $dbconnection->query("SELECT pk, item FROM diaryItems");
        while ($row = $query->fetch_assoc()) {
            $rows[] = $row;
        }
        $response = array("status" => "OK", "msg" => "Items to add to the diary", "items" => $rows);
        returnJSON($response);
        $dbconnection->close();
    }
}

function getItemsUserConsumed($pathParts, $index) {
    $token = $pathParts[$index];
    $dbconnection = new mysqli("localhost", "cse383", "HoABBHrBfXgVwMSz", "cse383");
    if ($dbconnection->connect_errno) {
        databaseFailure();
    }
    else {
        // checks for token in database
        $query = $dbconnection->query("SELECT user FROM tokens WHERE token = '$token'");
        $userFromToken = ($query->fetch_assoc())['user'];
        if ($query->num_rows == 0) {
            $response = array("status" => "FAIL", "msg" => "Token not found");
            returnJSON($response);
            $dbconnection->close();
        }
        else if ($query->num_rows >= 2) {
            $response = array("status" => "FAIL", "msg" => "More than 1 token found");
            returnJSON($response);
            $dbconnection->close();
        }
        else {
            // find the user that goes with the above token
            $query->free();
            $query = $dbconnection->query("SELECT pk FROM users WHERE user = '$userFromToken'");
            $pkFromUser = ($query->fetch_assoc())['pk'];
            $query->free();
            $query = $dbconnection->query("SELECT diaryItems.pk as pk, diaryItems.item as item, timestamp FROM diaryItems LEFT JOIN diary on diaryItems.pk = diary.itemFK WHERE userFK = '$pkFromUser' ORDER BY timestamp DESC");
            while ($row = $query->fetch_assoc()) {
                $rows[] = $row;
            }
            $response = array("status" => "OK", "msg" => "Diary populated", "items" => $rows);
            returnJSON($response);
            $dbconnection->close();
        }
    }
}

function getSummaryOfItems($pathParts, $index) {
    $token = $pathParts[$index];
    $dbconnection = new mysqli("localhost", "cse383", "HoABBHrBfXgVwMSz", "cse383");
    if ($dbconnection->connect_errno) {
        databaseFailure();
    }
    else {
        // checks for token in database
        $query = $dbconnection->query("SELECT user FROM tokens WHERE token = '$token'");
        $userFromToken = ($query->fetch_assoc())['user'];
        if ($query->num_rows == 0) {
            $response = array("status" => "FAIL", "msg" => "Token not found");
            returnJSON($response);
            $dbconnection->close();
        }
        else if ($query->num_rows >= 2) {
            $response = array("status" => "FAIL", "msg" => "More than 1 token found");
            returnJSON($response);
            $dbconnection->close();
        }
        else {
            // find the user that goes with the above token
            $query->free();
            $query = $dbconnection->query("SELECT pk FROM users WHERE user = '$userFromToken'");
            $pkFromUser = ($query->fetch_assoc())['pk'];
            $query->free();
            $query = $dbconnection->query("SELECT diaryItems.item as item, count(timestamp) as count FROM diaryItems LEFT JOIN diary on diaryItems.pk = diary.itemFK WHERE userFK = '$pkFromUser' GROUP BY diaryItems.item");
            while ($row = $query->fetch_assoc()) {
                $rows[] = $row;
            }
            $response = array("status" => "OK", "msg" => "Items the user has entered", "items" => $rows);
            returnJSON($response);
            $dbconnection->close();
        }
    }
}


function getToken($jsonData) {
    if (!isset($jsonData["username"]) || !isset($jsonData["password"])) {
        $response = array("status" => "FAIL", "msg" => "Insufficient data given");
        returnJSON($response);
    }
    else {
        $dbconnection = new mysqli("localhost", "cse383", "HoABBHrBfXgVwMSz", "cse383");
        if ($dbconnection->connect_error) {
            databaseFailure();
        }
        else  {
            // authenticate first
            $user = $jsonData["username"];
            $pass = $jsonData["password"];
            $query = $dbconnection->query("SELECT password FROM users WHERE user = '$user'");
            $result = $query->fetch_assoc();
            if ($result == NULL || !password_verify($pass, $result['password'])) {
                $response = array("status" => "FAIL", "msg" => "Could not authenticate");
                returnJSON($response);	
            }
            else {
                // delete any existing tokens
                $dbconnection->query("DELETE FROM tokens WHERE user = '$user'");
                // create a new token
                $token = uniqid();
                $query->free();
                if ($query = $dbconnection->prepare("INSERT INTO tokens (user, token) VALUES (?, ?)")) {
                    $query->bind_param("ss", $user, $token);
                    $query->execute();
                    $response = array("status" => "OK", "msg" => "Inserted into database properly", "token" => $token);
                    returnJSON($response);
                }
                else {
                    $response = array("status" => "FAIL", "msg" => "Could not insert into database");
                    returnJSON($response);
                }
                $dbconnection->close();
            }
        }
    }
}

function updateItemsConsumed($jsonData) {
    if (!isset($jsonData["token"]) || !isset($jsonData["itemFK"])) {
        $response = array("status" => "FAIL", "msg" => "Insufficient data given");
        returnJSON($response);
    }
    else {
        $dbconnection = new mysqli("localhost", "cse383", "HoABBHrBfXgVwMSz", "cse383");
        if ($dbconnection->connect_error) {
            databaseFailure();
        }
        else {
            // checks for token in the database
            $token = $jsonData["token"];
            $query = $dbconnection->query("SELECT user FROM tokens WHERE token = '$token'");
            $userFromToken = ($query->fetch_assoc())['user'];
            if ($query->num_rows == 0) {
                $response = array("status" => "FAIL", "msg" => "Token not found");
                returnJSON($response);
            }
            else if ($query->num_rows >= 2) {
                $response = array("status" => "FAIL", "msg" => "More than 1 token found");
                returnJSON($response);
            }
            else {
                $query->free();
                $query = $dbconnection->query("SELECT pk from users where user = '$userFromToken'");
                $pkFromUser = ($query->fetch_assoc())['pk'];
                $itemFK = $jsonData["itemFK"];
                $query->free();
                if ($query = $dbconnection->prepare("INSERT INTO diary (userFK, itemFK) VALUES (?, ?)")) {
                    $query->bind_param("ss", $pkFromUser, $itemFK);
                    $query->execute();
                    $response = array("status" => "OK", "msg" => "Inserted into database properly");
                    returnJSON($response);
                }
                else {
                    $response = array("status" => "FAIL", "msg" => "Could not insert into database properly");
                    returnJSON($response);
                }
                $dbconnection->close();
            }
        }
    }
}

?>
