access_token=<你的權杖>
message=<發送的訊息>
curl -s -X POST -H “Authorization: Bearer $access_token” -F “message=$message” https://notify-api.line.me/api/notify
