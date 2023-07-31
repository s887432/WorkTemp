#!/bin/bash
access_token=$(<token.txt)  
message=$1
curl -s -X POST -H "Authorization: Bearer $access_token" -F "message=$message" https://notify-api.line.me/api/notify
