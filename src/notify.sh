#!/bin/bash

TELEGRAM_API_TOKEN="6967275596:AAFdVF4YQZ1D_tp-yfy0UHM2WVj68yl87_4"
CHAT_ID="4186178789"

URL="https://api.telegram.org/bot$TELEGRAM_API_TOKEN/sendMessage"
TEXT="Deploy status: $1%0AProject name: $CI_PROJECT_NAME%0AURL: $CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/"

curl -s -d "chat_id=$CHAT_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null